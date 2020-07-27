/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#include <ImageImporter/ImageImporter.h>
#include <LogMacro.h>
#include <stb_image.h>

namespace Berserk {

    ImageImporter::ImageImporter() {
        mRecognizedExtensions.ensureToAdd(9);
        mRecognizedExtensions.emplace("jpeg");
        mRecognizedExtensions.emplace("jpg");
        mRecognizedExtensions.emplace("png");
        mRecognizedExtensions.emplace("tga");
        mRecognizedExtensions.emplace("bmp");
        mRecognizedExtensions.emplace("gif");
        mRecognizedExtensions.emplace("pic");
        mRecognizedExtensions.emplace("ppm");
        mRecognizedExtensions.emplace("pgm");

        registerImporter();
        BERSERK_LOG_INFO("Initialize ImageImporter (STB image)");
    }

    ImageImporter::~ImageImporter() {
        unregisteImporter();
        BERSERK_LOG_INFO("Finalize ImageImporter (STB image)");
    }

    EError ImageImporter::import(TPtrShared<Resource> &resource, const CString &importPath, const TPtrShared<ResourceImportOptions> &options) {
        Guard scopedLock(mAccessMutex);

        const ImageImportOptions* imageImportOptions = Image::getDefaultImportOptions().getPtr();

        if (options.isNotNull()) {
            auto optionsPtr = options.getPtr();
            auto imageOptionsPtr = dynamic_cast<const ImageImportOptions*>(optionsPtr);

            if (imageOptionsPtr)
                imageImportOptions = imageOptionsPtr;
        }

        auto pixelFormat = imageImportOptions->getFormat();
        auto width = imageImportOptions->getWidth();
        auto height = imageImportOptions->getHeight();
        auto isSourceInSRGB = imageImportOptions->getFromSRGB();
        auto isResultInSRGB = imageImportOptions->getKeepSRGB();

        int32 imported_w;
        int32 imported_h;
        int32 imported_channels;
        int32 desired_channels;

        switch (pixelFormat) {
            case EPixelFormat::R8G8B8A8:
                desired_channels = 4;
                break;
            case EPixelFormat::R8:
                desired_channels = 1;
            default:
                BERSERK_ERROR_RET_VALUE(EError::FAILED_IMPORT_RESOURCE, "Unsupported desired pixel format for path [%s]", importPath.data());
        }

        unsigned char *data = stbi_load(importPath.data(), &imported_w, &imported_h, &imported_channels, desired_channels);
        BERSERK_COND_ERROR_RET_VALUE(EError::FAILED_IMPORT_RESOURCE, data != nullptr, "Failed to load image [%s]", importPath.data())

        TPtrShared<Image> image = TPtrShared<Image>::make();
        image->create(imported_w, imported_h, pixelFormat, data, isSourceInSRGB);

        if (width != 0 && height != 0) {
            // Resize an image to desired size
            // Note: this transformation may scale an image for axis x/y
            image->resize(width, height);
        }

        if (isSourceInSRGB && !isResultInSRGB) {
            // Convert an image from sRGB space to gamma space
            // Note: if image was not actually in sRGB, then it will appear more dark
            image->convertToLinearSpace();
        }

        if (!isSourceInSRGB && isResultInSRGB) {
            // Convert an image from gamma space to sRGB space
            image->convertToSRGB();
        }

        stbi_image_free(data);

        resource = (TPtrShared<Resource>) image;
        return EError::OK;
    }

    const TArray<CString> &ImageImporter::getRecognizedExtensions() const {
        return mRecognizedExtensions;
    }

    const char *ImageImporter::getImporterName() const {
        static const char importerName [] = "ImageImporter";
        return importerName;
    }

    const char *ImageImporter::getImporterProjectName() {
        static const char importerProjectName [] = "BerserkImporters";
        return importerProjectName;
    }
}