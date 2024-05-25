﻿# Smart_Allocator

 This project has a template structure smart_allocator<T>. It is RAII struct that holds the pointer to T type object. It provides a mechanism for managing the dynamic resources. 

 - To use it in external projects copy the smart_alloc.h header file with Interface functions definitions. Then create folder with name include, put there header file. Then in external project properties in section Additional Included Directories add the path to the include folder.
 - Then add #include"smart_alloc.h" to the external project.

smart_allocator<T> is the template class so no .lib or .obj files will not be created, cause we have no .cpp file. And while the template gets specified with a certain value the instantination process will take place. And new struct with this type will be generated!
