#include "compare.h"

int compareWords(const char* word1, const char* word2) {
	int i = 0;
	while (word1[i] != 0 && word2[i] != 0) {
		char char1 = word1[i];
		char char2 = word2[i];

		if (char1 >= 'A' && char1 <= 'Z') {
			char1 += 32;
		}
		if (char2 >= 'A' && char2 <= 'Z') {
			char2 += 32;
		}

		if (char1 < char2) {
			return -1;
		}
		
		if (char1 > char2) {
			return 1;
		}

		i++;
	}


	if (word1[i] == 0 && word2[i] != 0) {
		return 1;
	}
	
	if (word1[i] != 0 && word2[i] == 0) {
		return -1;
	}
	
	return 0;
}
