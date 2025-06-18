/* Q1

You operate several hot dog stands distributed throughout town. 
Define a class named HotDogStand that has a member variable for the hot dog stand’s ID number and a member variable for how many hot dogs the stand sold that day. 
Create a constructor that allows a user of the class to initialize both values.

Also create a function named JustSold that increments the number of hot dogs the stand has sold by one. 
This function will be invoked each time the stand sells a hot dog so that you can track the total number of 
hot dogs sold by the stand. Add another function that returns the number of hot dogs sold.

Finally, add a static variable that tracks the total number of hot dogs sold by all hot dog stands 
and a static function that returns the value in this variable. Write a main function to test your 
class with at least three hot dog stands that each sell a variety of hot dogs.




*/


/* Q2
In an ancient land, the beautiful princess Eve had many suitors. She decided on the following procedure to determine which suitor she would marry. First, all of the suitors would be lined up one after the other and assigned numbers. The first suitor would be number 1, the second number 2, and so on up to the last suitor, number n. Starting at the first suitor she would then count three suitors down the line (because of the three letters in her name) and the third suitor would be eliminated from winning her hand and removed from the line. Eve would then continue, counting three more suitors, and eliminating every third suitor. When she reached the end of the line she would continue counting from the beginning.

For example, if there were six suitors then the elimination process would proceed as follows:

123456 initial list of suitors, start counting from 1

12456 suitor 3 eliminated, continue counting from 4

1245 suitor 6 eliminated, continue counting from 1

125 suitor 4 eliminated, continue counting from 5

15 suitor 2 eliminated, continue counting from 5

1 suitor 5 eliminated, 1 is the lucky winner

Write a program that uses a vector to determine which position you should stand in to marry the princess if there are n suitors. You will find the following function from the Vector class useful:

v.erase(iter);

// Removes element at position iter

For example, to use this function to erase the fourth element from the beginning of a vector variable named theVector, use theVector.erase(theVector.begin( ) + 3);

The number 3 is used because the first element in the vector is at index position 0.

*/