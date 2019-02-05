# Core System

Engine core system provides the most common and primary basic features for building applications and another engine's sus-systems. Evolves as subset of features for working with math, strings, XML, time, platforms and drivers configuration,
containers and threads. Provides sufficient functionality in memory managing, allows to omit platform malloc / free
and use engine build-in specialized pool, stack, linear and list allocators. 

## Submodules overview

### Math

* Vectors
* Matrices
* Quaternions
* Math utils
* Transformations
* Rotation gizmo
* Projections
* Sphere
* Axis aligned bounding box
* Plane
* Frustum
* Consts and thresholds

### Misc

* Types
* Assert
* Platform defines
* Macro
* Hash functions
* Safe cast
* Safe delete
* Buffers and related consts

### Memory

* Platform allocator
* Pool allocator
* List allocator
* Linear allocator
* Stack allocator
* Tagged heap allocator
* Chunk allocator

### Logging

* Log macro
* Log manager
* Log verbosity

### Profiling

* Memory measurement
* Time / performance measurement
* Per system memory usage
* Per mananger memory usage

### Threading

### Time

### Strings

* Static strings
* Dynamic strings
* Hashed string
* Wide character strings
* String pool
* String table
* String utils
* String builder

### Config

* Config format
* Config loader
* Config table
* Per system config support
* Per object config support

### Containers

* Linked list
* Shared list
* Queue
* Array list
* Hash table