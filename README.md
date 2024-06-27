the code for linear algebra based on Dr.John Stockie's lectures
http://www.sfu.ca/~jstockie/

it is fun to do this, if you tell me to make a website, yeh, i know how to do it, but i would rather shoot myself in the foot.
# Linear Algebra
A class perform basic linear algebra. I will insert animation later on so that it is easier to visual. it is quite easy to just understand the number aspect of linear algebra. the hard point is that it is hard to understand the geometric.  

`Matrix<T>(unsigned int, unsigned int)` default constructor, declare a matrix with designated size, with all 0 inside  
@params:  
`unsigned int`: x dimention, number of columns.  
`unsigned int`: y dimention, number of rows.  

`Matrix<T>(unsigned int, unsigned int, const T*)` declare a matrix with deisnated size, and copy down the elements of the given array  
@params:  
`unsigned int`: x dimention, number of columns.  
`unsigned int`: y dimention, number of rows.  
`const T*`: the array decay to pointer to copy.

`Matrix<T>(Matrix<T>*)` a copy constructor  
@params:  
`Matrix<T>*`: poitner to the matrix to copy  

`T get(size_t, size_t)` get element at specified location  
@params:  
`size_t` unsinged integer for x (column) in the table
`size_t` unsigned integer for y (row) in the table  
@return:  
`T` the data at the indicated cell  
@error:  
`string` error message, index is out of bound

an inllustration for LU decomposition:  
![](https://github.com/ARandomStrangerr/cpp/blob/main/linear/LU%20illustration.gif)
  
all the illustrations is scripted on [Google Colab-notebook](https://colab.research.google.com/drive/1w-wrmK5GKy6GDv7AoIVimvzwBzArBPJw?usp=sharing)

# Particle Simulation
An implementation of Verlet Integration
Gen 1: down to 20 FPS with roughly 300 Objs
![](https://github.com/ARandomStrangerr/cpp/blob/main/particle-sim/gen1%20demonstration.gif)
