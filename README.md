# Ian Bell code examples

Current Collection of programs that I feel show my programing skills and knowledge. 
Most of the code was submitted to CU for computer science classes.

Webserver: Uses socket I/O to to perform get requests for webpages found in "/www", specifically the index homepage and its resources. (The webpage and other resources in "/www" were not created by me)

Webproxy: Using socket I/O the proxy handles concurent requests for webpages, caches locally, and checks requests against a blacklist. 

Distribusted_File_System: Client and server programs that run a distributed file system. Users can add and pull files from the 4 servers which each hold 2 pieces of anygiven file. This allows for 2 servers to file before files become un-available to users.  

Bell_Box: Creates the smallest possible bounding box for a set of points.

Bell_DynamicPandaPeril: Program demonstrates dynamic vs greedy approaches to programming. 
"In the two-player game “Pandas Peril”, an even number of cards are laid out in a row,
face up. On each card, is written a positive integer. Players take turns removing a
card from either end of the row and placing the card in their pile. The player whose
cards add up to the highest number wins the game." Player 2 demonstrates the greedy approach to the game, picking the highest card possible at that time, everytime. Player 1 demonstrates a dynamic aproach to the game, picking cards based on the end value of their hand. The program calculates the dynamic choices and simulates the game to demonstrate that dynamic approaches can be at least as effective as greedy. 

Bell_PathCount: Uses Djikstra's algorithm to calculate the number of paths along a "spider-web" graph. 
