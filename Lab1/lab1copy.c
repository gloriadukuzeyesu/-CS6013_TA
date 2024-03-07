/*********************************************************************
 * CS6013_ Spring 2024
 * Jan 08th, 2023
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h> // For strlen()
#include <assert.h>
/*********************************************************************
 *
 * The C functions in this lab use patterns and functionality often found in
 * operating system code. Your job is to implement them.  Additionally, write some test
 * cases for each function (stick them in functions [called xyzTests(), etc or similar])
 * Call your abcTests(), etc functions in main().
 *
 * Write your own tests for each function you are testing, then share/combine
 * tests with a classmate.  Try to come up with tests that will break people's code!
 * Easy tests don't catch many bugs! [Note this is one specific case where you may
 * share code directly with another student.  The test function(s) from other students
 * must be clearly marked (add '_initials' to the function name) and commented with the
 * other student's name.
 *
 * Note: you may NOT use any global variables in your solution.
 *
 * Errata:
 *   - You can use global vars in your testing functions (if necessary).
 *   - Don't worry about testing the free_list(), or the draw_me() functions.
 *
 * You must compile in C mode (not C++).  If you compile from the commandline
 * run clang, not clang++. Submit your solution files on Canvas.
 *
 *********************************************************************/

/*********************************************************************
 *
 * byte_sort()
 *
 * specification: byte_sort() treats its argument as a sequence of
 * 8 bytes, and returns a new unsigned long integer containing the
 * same bytes, sorted numerically, with the smaller-valued bytes in
 * the lower-order byte positions of the return value
 *
 * EXAMPLE: byte_sort (0x0403deadbeef0201) returns 0xefdebead04030201
 * Ah, the joys of using bitwise operators!
 *
 * Hint: you may want to write helper functions for these two functions
 *
 *********************************************************************/

// Global variables for testing purposes only
unsigned long test_cases[] = {
	0x0403deadbeef0201,
	0x1122334455667788,
	0x5566778811223344,
	0xdbeef02010403dea,
	0x1111111111111111,
	0x0000000300040000,
	0x0000000000000001,
	0xaabbccddeeff1122};


// bubble sort
void bubble_sort_byte(unsigned char bytes[], size_t n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (bytes[j] > bytes[j + 1])
			{
				unsigned char temp = bytes[j];
				bytes[j] = bytes[j + 1];
				bytes[j + 1] = temp;
			}
		}
	}
}

unsigned long byte_sort(unsigned long arg)
{
	unsigned char bytes[8]; // each char occupies 1 byte of memory. long occupies 8bytes.

	for (int i = 0; i < 8; i++)
	{
		bytes[i] = (arg >> i * 8) & 0xFF;
	}
	// sort
	// can use bubble sort with O(n^2) time complexity
	bubble_sort_byte(bytes, 8);
	// Step3. Reconstruct the unsigned long arg from the sorted unsigned char bytes array
	unsigned long sorted_integer = 0;
	for (int i = 7; i >= 0; i--)
	{
		sorted_integer = (sorted_integer << 8) | bytes[i];
	}
	return sorted_integer;
}

/*********************************************************************
 *
 * nibble_sort()
 *
 * specification: nibble_sort() treats its argument as a sequence of 16 4-bit
 * numbers, and returns a new unsigned long integer containing the same nibbles,
 * sorted numerically, with smaller-valued nibbles towards the "small end" of
 * the unsigned long value that you return
 *
 * the fact that nibbles and hex digits correspond should make it easy to
 * verify that your code is working correctly
 *
 * EXAMPLE: nibble_sort (0x0403deadbeef0201) returns 0xfeeeddba43210000
 *
 *********************************************************************/

//**** my implementation **********////
unsigned long nibble_sort_gloria(unsigned long arg)
{
	unsigned char bits[16];
	for (int i = 0; i < 16; i++)
	{
		bits[i] = (arg >> i * 4) & 0xF;
		// printf("0x%x\n", bits[i]); // 0x%lx\n
	}
	bubble_sort_byte(bits, 16);
	unsigned long sorted_integer = 0;
	for (int i = 15; i >= 0; i--)
	{
		sorted_integer = (sorted_integer << 4) | bits[i];
	}

	return sorted_integer;
}

// ****** end of my implementation ***** 


unsigned long nibble_sort(unsigned long input) {
  
  // Define a constant representing the bit pattern '0001' repeated 16 times
  const unsigned long long NIBBLE_MASK = 0x1111111111111111ULL;
  
  // Check if the input is already nibble sorted
  if (input == (input & 0xF) * NIBBLE_MASK) {
    // If sorted, no need to perform sorting, return the input as it is
    return input;
  }

  // Initialize a variable to count occurrences of each nibble
  unsigned long long nibble_count = 0;

  // Iterate through each nibble (4 bits) in the input
  for (int i = 0; i < 16; i++) {
    // Increment the count for the current nibble
    // (input & 0xF) extracts the least significant nibble (4 bits) from the variable input. The 0xF bitmask is used to retain only the lowest 4 bits, discarding the higher bits.
    //Multiplying the extracted nibble by 4 determines the correct position in the 64-bit nibble_count variable.
    // 1ULL << (4 * (input & 0xF)) shifts the binary 1 by 4 times the value of the extracted nibble, effectively setting a single bit at the calculated position
    nibble_count += 1ULL << (4 * (input & 0xF));
    
    // Move to the next nibble
    input >>= 4;
  }

  // Initialize a mask with all bits set to 1
  unsigned long long mask = ~0ULL;
  
  // Initialize the result variable
  unsigned long long result = 0;

  // Iterate through each nibble count
  for (int i = 0; i < 16; i++) {
    // Extract the least significant nibble from the count
    int current_nibble_count = nibble_count & 0xF;
    
    // Shift the count to get the next nibble count
    nibble_count >>= 4;

    // Update the result using the current nibble count and mask
    result = (result & ~mask) | ((NIBBLE_MASK & mask) * i);
   
    // Shift the mask to the left based on the current nibble count
    mask <<= 4 * current_nibble_count;
  }

  // Return the nibble-sorted result
  return result;
}




//*********************************************************************/

typedef struct elt
{
	char val;
	struct elt *link;
} Elt;

/*********************************************************************/

/* Forward declaration of "free_list()"... This allows you to call   */
/* free_list() in name_list() [if you'd like].                       */
void free_list(Elt *head); // [No code goes here!]

/*********************************************************************
 *
 * name_list()
 *
 * specification: allocate and return a pointer to a linked list of
 * struct elts
 *
 * - the first element in the list should contain in its "val" field the first
 *   letter of your first name; the second element the second letter, etc.;
 *
 * - the last element of the linked list should contain in its "val" field
 *   the last letter of your first name and its "link" field should be a null
 *   pointer
 *
 * - each element must be dynamically allocated using a malloc() call
 *
 * - you must use the "name" variable (change it to be your name).
 *
 * Note, since we're using C, not C++ you can't use new/delete!
 * The analog to delete is the free() function
 *
 * - if any call to malloc() fails, your function must return NULL and must also
 *   free any heap memory that has been allocated so far; that is, it must not
 *   leak memory when allocation fails
 *
 * Implement print_list and free_list which should do what you expect.
 * Printing or freeing a nullptr should do nothing.
 *
 * Note: free_list() might be useful for error handling for name_list()...
 *
 *********************************************************************/

Elt *name_list()
{
	// The malloc() function allocates size bytes of memory
	// and returns a pointer to the allocated memory.

	// The free() function deallocates the memory allocation pointed to by ptr.
	// If ptr is a NULL pointer, no operation is performed.

	char *name = "Gloria";
	Elt *head_of_list = NULL;
	Elt *current_node = NULL;

	// iterate throuh the name and dynamically create an Elt object for each character
	// strlen(name) 
	for (int i = 0; name[i] != '\0'; i++)
	{
		// dynamically allocate a block of memory with the size of Elt.
		Elt *new_elt_ptr = (Elt *)malloc(sizeof(Elt));

		if (new_elt_ptr == NULL)
		{ // there is an error when malloc returns a null pointer
			free_list(head_of_list);
			return NULL;
		}

		new_elt_ptr->val = name[i];
		new_elt_ptr->link = NULL;

		// verify id the list is empty. if it is, then the new_elt_ptr becomes the head  of the list
		if (head_of_list == NULL)
		{
			head_of_list = new_elt_ptr;
			current_node = head_of_list;
		}
		else
		{
			// contiuaton of the build of the list
			current_node->link = new_elt_ptr;
			current_node = current_node->link;
		}
	}

	return head_of_list;
}

/*********************************************************************/

void print_list(Elt *head)
{
	Elt *current = head;
	while (current != NULL)
	{
		printf("%c", current->val);
		current = current->link;
	}
	printf("\n");
}

/*********************************************************************/

void free_list(Elt *head)
{
	Elt *temp_elt_var;
	while (head != NULL)
	{
		temp_elt_var = head;
		head = head->link;
		free(temp_elt_var);

	}
	// The free() function deallocates the memory allocation pointed to by ptr.
	// If ptr is a NULL pointer, no operation is performed.
	// source : man malloc
}

/*********************************************************************
 *
 * draw_me()
 *
 * This function creates a file called 'me.txt' which contains an ASCII-art
 * picture of you (it does not need to be very big).
 *
 * Use the C stdlib functions: fopen, fclose, fprintf, etc which live in stdio.h
 * - Don't use C++ iostreams
 *
 *********************************************************************/

void draw_me()
{
	FILE *file_ptr = fopen("me.txt", "w");
	if (file_ptr == NULL)
	{
		printf("%s", "failed to create/open the me.txt file");
		return;
	}
	const char *my_ascii_art = "\
	::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::^^^^^^^^^^^^^^^^\n\
	:::::::::::::::::::::::::.:.:......................:..:::::::::::::::::::::::::::::::^^^^^^^^^^^^^^^\n\
	::::::::::::::::::.....:...................................:::::::::::::::::::::::::::^^^^^^^^^^^^^^\n\
	:::::::::::::::...:............................................:.::::::::::::::::::::::::^^^^^^^^^^^\n\
	::::::::::::.......................................................:::::::::::::::::::::::^^^^^^^^^^\n\
	:::::::::.:............................................................:::::::::::::::::::::^^^^^^^^\n\
	:::::::.:...............................................................:::::::::::::::::::::^^^^^^^\n\
	::::...........................................^~..........................::::::::::::::::::::^^^^^\n\
	:.....................................^:B#&###B###&BB7:....................::::::::::::::::::::^^^^^\n\
	..................................7#&#BGB##&&###&&#B#&&&&B5...................:::::::::::::::::::^^^\n\
	...............................^P##&#BBBB&#BB##&#&B#&&B#&#&Y:..................:::::::::::::::::^:^^\n\
	.............................!5BB5B##&###BG###&##B###&##&#B####B...............:::::::::::::::::^^^^\n\
	...........................!B#BBBBBBBBPBBGGGPGBBB#B##BB###&&##B&#BJ:.............::::::::::::::::::^\n\
	..........................:GBGGPGPJP55PPY5JJ555PGGBBB#BBBB#BG#&&##&#&#7...........:::::::::::::::::^\n\
	.........................:PGGP555?YJYJYJJJJYJYYJPY5GPGBBG##&##&&##&&##&&Y:.........:::::::::::::::::\n\
	........................PGGYYJJJJ???J?77JJJJ??J?YYYY5PPBBBBGG##GB&&###&&&B^...:.....:::::::::::::::^\n\
	......................:PG5YJJJ?JJ?7!~~~7??7!J~JJJ5YYYJ5PPG#B####&&&G####&&&#~.......::::::::::::::::\n\
	......................^G5?JJ?? ?!7!~77~? ?!!7?!~?JY!JJYJY5PGGBBBGB#&##&@@##&&&#:......::::::::::::::::\n\
	......................JPJJJY?7!~!?~!~?~?!777J7???JJJJJYY55PG#BBB#B#&&&&#&&&&&&J:.....:::::::::::::::\n\
	.....................:PYJ???? ?!??P!7!!~7!~!77~757?JJ?JYY5Y5PPBGBBBB&&####&&&&&#:...::::::::::::::::^\n\
	.....................^GJJJ?7J?77^::!^!^?7!~^^777?7??7JJJJ?YY5PPBBB##&#B#&#&&&&&P:...::::::::::::::::\n\
	......................Y?7777!7?^!7!J?!?!??57PBPGBJPJ#PBGJYYYYY5PPG#&#B#&&&&&&&&&J:.:::::::::::::::::\n\
	......................J7?PBPGPPPY?!?J7!??7J?JJYY?Y7~777YJ5JYYY5PGGB#B#&&&@@&&&&&5::.::::::::::::::::\n\
	.....................:5YYJYYP5YYYJ5J?!!?J!5JJ7?~7?7?JJJ?YJYJJYY5GBBB##&@@@@&&&&&#^:::::::::::::::::^\n\
	.......................?JJ? ?!^J??Y5YJ!!7!?J!?Y55JB##BYY5J5YJJYJYPPBB#5P&@@@@@&&&&B:.:::::::::::::^^^\n\
	......................:75PGB#&&#GYYY7~~!J??~P5!~^B#5G5PYJ?JYJJYY5GPP5G#&&@@@&&&&&&J::::::::::::::^^^\n\
	......................!?YG77!BBG~7JJ7777~??J7J?7YYYJJ777!?JJ?JYYY55P55GB@@@@@@&@&&J^::::::::::::^^^^\n\
	......................7???JYYY!?~???77!7?77?7? ?!??7J7^!!7J?JYJYY55PBG5P&&@@@&&&&&&#^::::::::::^^^^^^\n\
	......................7777!!7!?7!?J? ?!!77JYJ7?!^~7~7!!7!?7?JJJYYY55PG5G&&@@@@&@&&&#G^:::::::^^^^^^^^\n\
	......................777!77777Y7Y77!::~77?7?577~!~~!~!777???JJY55B55P&&&&@@@@&&#&PG?::::::^^^^^^^^^\n\
	......................77777!!^!??7JJ7??JY&&&PP~J!7777?!7Y5JJYYYY5P55P@@&&&@@@@&&&#5^::::::^^^^^^^^^^\n\
	......................~7?77!!!!?7?JYJY5G??J~J?7J?Y!JJ7???YYYYYY55PP5G@@&&&@@@@&&&@&^:::::^^^^^^^^^^~\n\
	.......................?????77?YJ???J? ?!!JJ7YJJ57YYYY5YY555J5Y555BBY@@@@&@@@@@&&&&#?^::^^^^^^^^^~~~~\n\
	........................?JJJYJJ5PYY?YYJ!?J?55JJYJ5PYJYJ5YYYYYYY5P5@5&@@@@&@@@&&&&&#G^^:^^^^^^^~~~~~~\n\
	.......................~.JJYYYYP5P555P5J75P5GPY?G#5??5YJJYYYY55PG?@&^@@&&@@&G&&&&#&B!^^^^^^^^~~~~~~~\n\
	..................... ....JJJY5YJYYJ^:.. .J:5JJJYYJJ?JJYJJY555PGG?@@.&&&&&#&&&#&&#&&P~^^^^^^~~~~~~!!\n\
	.....................~.....?JJJJJ7?J?J???77!7JJYYYJJJJYYYY5PPGGGG!@@!&&&@&@@&&&&@&&@&&?^^^~~~~~~!~!!\n\
	....................^^.......JYJJJJ?YYYJYJ7555JYY?JYJJYY55PGBBGPPBP&7&&@&@&&&&&@&5B#@&P~^^~~~~~~!!!!\n\
	.....................^.........JYJJJJYJ5555PYJYY????JY55PGBBBGGGP#P##&&&&&GJ@&@&#B?5&P#!^~~~~~~~!!!!\n\
	......................77........~JJJ??? ?!!7~^~?J7JYYY5GGB#BBGGPPPB&B&&@&&&@&G@@&B7^5&YJ&57~~~~!!!!!!\n\
	........................^!!?JJ?^:G&GJ?JJ?!7!!?YYJ55PGB##BBGP5555PPG@@@@&&&@@@G&&~~7~#&7JP?7!~~!!!!!7\n\
	................................:B&JJPYYYYYYY55PPBB##BBBPP55YYY555PG&@##&&G~7&#G~7~^~G&GG!~~~~!!!!!!\n\
	................................~#&&&PYY55PGBBBBBBBGGGP5YYYYJYYY55PG##&##&##&&&&#!^^^~B&#!~~~!!!!!!!\n\
	...............................^P#&@&YYYYY55555PPPPP5YYJJJJJJJYYY?#########&&&&@B!^^~J&&Y~~~!!!!!!!!\n\
	...............................?#&@P!JYYYYYYYYYYYYYYJ??J?J??JJJ?5#############&&J^^^?~!#P!~!!!!!!!77\n\
	...............................J#&# :5JYYYYYYYYYYJJJ?J?JJ?JJ?!J##############&&##BG?~!GPB?!!!!!!!777\n\
	:..:........................:JBB&#?...5JYYYJYJJJJ?J????JJ????G###############&###&&&##PP&?!!!!!!!!77\n\
	:....................:~5######B#&J~......~JYJYJ??~7??JJJ?J??#########################&#&&&#B!!!!7777\n\
	:::::..........:~5############B#B~:.   .....  ..... . . ..Y########################&####&&&&&###Y777\n\
	:::::.......:?B###############B#?^^::...   .         .  .#############################&&&##&&&&&&&&#\n\
	:::::::..:7BB################B##^::^^^^^^:....   . ::::5############################&####&&&###&#&&&\n\
	:::::::^5BB##################B#Y::::^:::^^^^::^.^:::^^####&#############&#&&###&##&##&&&&&&#&&&&#&&&\n\
	:::::^G#####&################B#^........:::::7  :::::################&#####&#####&#&&&&#&&#&&&&#&#&&\n\
	::::5#######&##################:..   ......:: :::...#############&&########&&##&B#&#&&##&&&&&&#&&#&#\n\
	:::P#####&#&###################..       ....:::...:##########B&&#&#######&&&#&###&#&&&&#&&&#&##&&#&&\n\
	::G####&&&#&##################G:.......    .   ..?#&############&#####&&&##&##&#&#&&&&&##&##&##&&&#&\n\
	:J#&&##&&&&&##################5::::::....    ...P######&####B####&&##&###########&&&&&&#&&&#&&&&&&&&\n\
	!#&&&&&&&&&###################!..  ....... . ..B##&#&######&##&########&#&&##&&#&##&&&&&&&&&&&#&&&&&\n\
	##&&&&&&&&&####&##############~.     .......::####&&####&#B#&&#B#&B&#&&&&&#&###&&&&&&&&&&&#&&&&&&&#&\n\
	#&&&&&&&&&####################!....:.........B#&&&#&#######&&#&B#&#&&&#&&##&&&&&&&&&&&&&&&&&#&&&&&&&\n\
	#&&&&&&&&&##&###########&#####~:............####&#&&#####&&&B#&#&#&#&&&&###&&&&#&&&&&#&&@@&&&&&&&#&&\n\
	&&&&&&&&&###&&&&###&#####&####.............##&&&#&######&#&#&#&&#&&&#&&&####&&#&#&&&&&&&@&@&&&&&&&&&\n\
	&&&&&&&&####&&&##&&&&#########:...........#&&#&##&&&###&##&&#B##&&#&##&#&&&&&&##&&&&&#&&@&&&&&&&&&&@\n\
	&&&&&&&&###&&&&#&&&&&&&##&&##P:..........J#&#&##&&##&B#&#&&##&#&&&&&&#&####&&&&&&&&&&&&@&&&&&&&&&&&&\n\
	&&&&&&&###&&&&#&&&&&&&&&&##&#Y~^^^^^^^^^^#&#&#&##B####&&#&&&#&#&&&&&&&&&&&&&&#&&&&&&&#&@@&&&&&&&&&&&\n\
	&&&&&&&##&&&&&&&&&&&&&&&&#&&&!...:::::^^#&#&&&&#&###&&&##G&&#&&&#@&#&&&&#&&&&&&&&&&&&&&@@&&&&&&&&&&&\n\
	&&&&&&#&#&&&&#&&#&&&&&&&&&#&&:.........#&&&&&#&&&####&&&&&&#&&&&&&&&&&#&&&&&&&&&&&&&&@@@&&&&&&&&&&&&\n\
	&&&&&&&#&&&&&&&&&&&&&&&&&&#&5::.:.:.::PB&&###&###B#&&#&&&##&&&#&&&&&&&&&&&&&&&&&&&&&&@@@#&&&&&&&&&&&\n\
	&&&&&&&&&&&&&&&&&&&&&&&&&&&&7..:....::&&#&#&&##&&####&&&&&&&&#&&&&&&&&&&&&&&&&&&&&&@@@@&&&&&&&&&&&&&\n\
	&&&&&&&#&&&&&&&&&&&&&&&&&&#&:........&&&&&&&&&&#&#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&@@@&&&&&@&&&&&&&\n\
	&&&&&&&&&&&&&&&&&&&&&&&&&&&G:.......#&&&&#&#&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&@@@&&&@&@&&&&&&&\n\
	";

	fprintf(file_ptr, "%s", my_ascii_art);
	fclose(file_ptr);
}

/*********************************************************************
 *
 * Test Code - Place your test functions in this section:
 */

void testByteSort()
{
	unsigned long expected_results_byte_sort[] =
		{
			0xefdebead04030201,
			0x8877665544332211,
			0x8877665544332211,
			0xf0eeeadb403d2010,
			0x1111111111111111,
			0x403000000000000,
			0x100000000000000,
			0xffeeddccbbaa2211};

	int test_number = 0;
	for (int i = 0; i < 8; i++)
	{
		unsigned long Output = byte_sort(test_cases[i]);
		// printf("Input:  0x%lx\n", test_cases[i]);
		// printf("Output: 0x%lx\n", Output);
		// printf("expected: 0x%lx\n", expected_results_byte_sort[i]);
		assert(Output == expected_results_byte_sort[i]);

		test_number++;
	}
	printf("%d / 8 tests passed\n", test_number);
}

void testNibbleSort()
{
	// unsigned long test_case = 0x0403deadbeef0201;
	// unsigned long res = nibble_sort(test_case);
	// printf("Output: 0x%lx\n", res);
	unsigned long expected_results_nibble_sort[] =
	{
		0xfeeeddba43210000,
		0x8877665544332211,
		0x8877665544332211,
		0xfeeeddba43210000,
		0x1111111111111111,
		0x4300000000000000,
		0x1000000000000000,
		0xffeeddccbbaa2211};

	int test_number = 0;
	for (int i = 0; i < 8; i++)
	{
		unsigned long Output = nibble_sort(test_cases[i]);
		// printf("Input:  0x%lx\n", test_cases[i]);
		// printf("Output: 0x%lx\n", Output);
		// printf("expected: 0x%lx\n", expected_results_nibble_sort[i]);
		assert(Output == expected_results_nibble_sort[i]);
		test_number++;
	}
	printf("%d / 8 tests passed\n", test_number);
}

void testNameList_freeList_printList()
{
	Elt *list = name_list();
	assert(list != NULL);
	print_list(list);
	free_list(list);
}

void test_draw_me()
{
	draw_me();
	// TODO add tests for draw()
}
/*********************************************************************
 *
 * main()
 *
 * The main driver program.  You can place your main() method in this
 * file to make compilation easier, or have it in a separate file.
 *
 *
 *********************************************************************/

int main()
{
	// Call your test routines here...
	/******1st part Sort()*******/
	testByteSort();
	testNibbleSort();

	/******2nd part LINKEDLIST*/
	Elt *list = name_list();
	print_list(list);
	testNameList_freeList_printList();

	/******3nd part draw_me()*/
	draw_me();
	printf("%s\n", "Done");
}
