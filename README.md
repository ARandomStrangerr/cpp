This project is just prime for fun.
# Json
## Introduction
This restructured as classes: `JsonPrimitive`, `JsonObject`, `JsonArray`
`JsonPrimitive` store pointer to a string.
`JsonObject` store given objects in a map.
`JsonArray` store given objects in a vector.
## Function
### `JsonObject.add(std::string, std::string*)`
add a string pointer to this Json Object.<br/>
input:<br/>
&emsp;std::string - name of the key<br/>
&emsp;std::string* - pointer to store under the above key (remember to allocate dynamically)
### `JsonObject.add(std::string, JsonObject*)`
add a JsonObject poitner to this Json Object.<br/>
input:<br/>
&emsp;std::string - name of the key<br/>
&emsp;JsonObject* - pointer to store under the above key (remember to allocate dynamically)
### `JsonObject.add(std::string, JsonArray*)`
add a JsonArray pointer to this Json Object<br/>
input:<br/>
&emsp;std::string - name of the key<br/>
&emsp;JsonArray* - pointer to store under the aboe key (remember to allocate dynamically)
### `JsonObject.get(std::string)`
get an element associates with the string
input:<br/>
&emsp;std::string - name of the key<br/>
output:<br/>
&emsp;JsonPrimitive* - pointer to the JsonPrimitive associate with the key<br/>
error:<br/>
&emsp;std::string - error message. this often because the given key is not associated with any thing.
### `delete JsonObject`
JsonObject destructor.<br/>
Calling this will traverse through the underlaying map and de-allocate all the pointers within this JsonObject.
### `JsonArray.add(std::string*)`
add a string pointer to this Json Array<br/>
input:<br/>
&emsp;std::string - string pointer to store (remember to allocate dynamically)
### `JsonArray.add(JsonObject*)`
add a JsonObject pointer to this JsonArray<br/>
input:<br/>
&emsp;JsonObject* - pointer to store (remember to allocate dynamically)
### `JsonArray.add(JsonArray*)`
add a JsonArray pointer to this JsonArray<br/>
input:<br/>
&emsp;JsonArray* - pointer to store (remember to allocate dynamically)
### `delete JsonArray`
JsonArray destructor<br/>
Calling this will iterate through the underlaying vector and de-allocate all the pointers within this JsonArray.<br/>
## Upcoming Ideas:
+ currently calling a destructor, if 2 or more places point to the same pointer, there will be malloc error of the pointer already de-allocated.
+ give a string then parse to `JsonObject` or `JsonArray`
# Socket
## Introduction
Reordering the plain TCP Socket (no encryption), and Listener to easier to use. Simply call:
+ `new Listener(int port, int backlog)` to open a listener.
+ `new Socket(std::string address, int port)` to connect to `<address>:<port>`
## Function
### `new Socket(std::string, int)`
constructor, create a Socket
input:
    std::string - ip address
    int - port
### `Socket.readLine()`
read a line from buffer. this function will be terminate when character '\n' is meet at the end of buffer
output:
    std::string* - pointer to a string. REMEMBER TO DELETE THIS WHEN DONE.
### `Socket.write(const char*, int)`
write an array of character tof buffer
input:
    const char* - pointer to char array, the easiest one is calling c_str() function
    int - lenght of the input char array

