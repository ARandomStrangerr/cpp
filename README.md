# Json
## Introduction
This restructured ass classes: `JsonPrimitve`, `JsonObject`, `JsonArray`
`JsonPrimitive` store pointer to a string.
`JsonObject` store given objects in a map.
`JsonArray` store given objects in a vector.
## Function
### add
JsonObject.add(std::string, std::string*)
JsonObject.add(std::string, JsonObject*)
JsonObject.add(std::string, JsonArray*)
JsonArray.add(std::string*)
JsonArray.add(JsonObject*)
JsonArray.add(JsonArray*)
the given std::string*, JsonObject*, or JsonArray* must be dinamically allocated.
### destructor
this recursively delete the object until reach a `JsonPrimitve` which holds a string.
## Upcoming Ideas:
+ currently calling a destructor, if 2 or more places point to the same pointer, there will be malloc error of the pointer already de-allocated.
+ give a string then parse to `JsonObject` or `JsonArray`
