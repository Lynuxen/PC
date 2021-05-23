#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "util/task_helper.h"
// KIRJAN DIMOVSKI 322CB ARS POETICA

// Mini functions to make the code look nicer down :)
bool is_lowercase(int character) {
	return character > 96 && character < 123 ? true : false;
}

bool is_uppercase(int character) {
	return character > 64 && character < 91 ? true : false;
}

bool is_alphabetic(int character) {
	return is_lowercase(character) || is_uppercase(character) ? true : false;
}

bool is_nonalphabetic(int character) {
	return character < 65 && character > 32 &&
	       character != 45 && character != 39 ? true : false;
}

bool is_space(int character) {
	return character == 32 ? true : false;
}

bool is_newline(int character) {
	return character == 10 ? true : false;
}

// Traverses the entire buffer, setting every character after a newline to
// uppercase, then checking every character afterwards if its a capital
// letter. If it is, then make it lowercase
void uppercase(char *buffer) {
	int i, newline_flag = 1, case_difference = 32;

	for (i = 0; i < strlen(buffer); ++i) {
		if (buffer[i] == '\n') {
			newline_flag = 1;
		} else if (newline_flag == 1 && is_lowercase(buffer[i])) {
			buffer[i] -= case_difference;
			newline_flag = 0;
		} else if (is_uppercase(buffer[i])) {
			if (newline_flag == 0) {
				buffer[i] += case_difference;
			} else {
				newline_flag = 0;
			}
		}
	}
}

// Traverses the buffer, checking for newspaces and non alphabetic characters.
// When the first of either is found, it continues checker if there is
// anything after (since there is only supposed to be one space and one
// period, comma etc).
void trimming(char *buffer) {
	int i, j, spaces_after = 0, non_alphabetic_after = 0;

	for (i = 0; i < strlen(buffer); ++i) {
		if (is_space(buffer[i])) {
			for (j = i + 1; j < strlen(buffer); ++j) {
				if (is_space(buffer[j])) {
					++spaces_after;
				} else {
					break;
				}
			}
			for (j = i; j < strlen(buffer); ++j) {
				buffer[j] = buffer[j + spaces_after];
			}
			spaces_after = 0;
		} else if (is_nonalphabetic(buffer[i])) {
			for (j = i + 1; j < strlen(buffer); ++j) {
				if (is_nonalphabetic(buffer[j])) {
					++non_alphabetic_after;
				} else {
					break;
				}
			}
			for (j = i; j < strlen(buffer); ++j) {
				buffer[j] = buffer[j + non_alphabetic_after + 1];
			}
			non_alphabetic_after = 0;
			--i;
		}
	}
}

// If it finds a letter, it calculates the probability and changes accordingly
void silly(char *buffer, float probability) {
	int i, case_difference = 32;
	float sample;

	for (i = 0; i < strlen(buffer); ++i) {
		if (is_alphabetic(buffer[i])) {
			sample = rand() % 100 / 99.0;
			if (sample < probability) {
				if (is_uppercase(buffer[i])) {
					buffer[i] += case_difference;
				} else if (is_lowercase(buffer[i])) {
					buffer[i] -= case_difference;
				}
			}
		}
	}
}

// Switches the word at the *position with the word from get_friendly_word,
// does fully since strstr returns a pointer to the first occurence of a
// substring, if they are consecutive. ex: If we have omusor then om,
// position will point to omusor.
void friendly(char *buffer) {
	char *token, *word, *temp_buffer;
	word = malloc(sizeof(char) * 30);
	temp_buffer = malloc(sizeof(char) * 100000);

	strcpy(temp_buffer, buffer);
	token = strtok(temp_buffer, " ,.\n");
	while (token != NULL) {
		get_friendly_word(token, &word);
		if (word != NULL) {
			char *position = strstr(buffer, token);
			if (position != NULL) {
				size_t new_length =
						strlen(buffer) - strlen(token) + strlen(word);
				char *new_buffer = malloc(sizeof(char) * new_length + 1);
				memcpy(new_buffer, buffer, position - buffer);
				memcpy(new_buffer + (position - buffer), word, strlen(word));
				strcpy(new_buffer + (position - buffer) + strlen(word),
				       position + strlen(token));
				memcpy(buffer, new_buffer, strlen(new_buffer) + 1);
				free(new_buffer);
			}
		}
		token = strtok(NULL, " ,.\n");
	}
	free(word);
}

// Copies the buffer into temp_buf so we can strtok it, starts parsing while
// checking if get_synonym is NULL. Enter the main part of the function,
// where depending on the type will traverse temp_buf2 so we can find the last
// letter to rhyme with. When it finds it, and if its different from the
// current letter, switches them (similar to friendly). Does not work if we
// need to change the second word of the pair, ex. in 1-2 3-4 only 1 and 3
// will be checked for possible switch.
void rhimy(char *buffer, char *type) {
	int i, length, position = 0, mode_1_4 = 1, newline_flag = 0, old_index = 0,
			min_lexic = 0, number_of_rhymes = 0, case_difference = 32;
	char letter_to_rhyme = 0, word_check[20],
			*token, *temp_buf, *temp_buf2, **words, **rhymes;

	memset(word_check, 0, sizeof(word_check));
	temp_buf = malloc(sizeof(char) * 100000);
	temp_buf2 = malloc(sizeof(char) * 100000);
	strcpy(temp_buf, buffer);
	strcpy(temp_buf2, buffer);

	words = (char **) malloc(sizeof(char *) * 30);
	for (i = 0; i < 30; ++i) {
		words[i] = (char *) malloc(sizeof(char) * 30);
	}

	rhymes = (char **) malloc(sizeof(char *) * 30);
	for (i = 0; i < 30; ++i) {
		rhymes[i] = (char *) malloc(sizeof(char) * 30);
	}

	token = strtok(temp_buf, " ,.\n");
	while (token != NULL) {
		get_synonym(token, &length, &words);
		if (words != NULL) {
			// Add every word from the buffer into word_check to see if its the word we need to compare
			if (strcmp(type, "imperecheata") == 0) {
				for (i = position; i <= strlen(temp_buf2); ++i) {
					if (is_space(temp_buf2[i])) {
						memset(word_check, 0, sizeof(word_check));
					} else if (is_newline(temp_buf2[i]) ||
					           temp_buf2[i] == '\0') {
						if (strcmp(token, word_check) == 0) {
							is_newline(temp_buf2[i + 1]) ? (i = i + 2) : ++i;
							while (!is_newline(temp_buf2[i]) &&
							       i <= strlen(temp_buf2)) {
								++i;
							}
							while (!is_alphabetic(temp_buf2[i])) {
								--i;
							}
							letter_to_rhyme = temp_buf2[i];
							if (is_uppercase(letter_to_rhyme)) {
								letter_to_rhyme += case_difference;
							}
							memset(word_check, 0, sizeof(word_check));
							break;
						}
					} else if (is_alphabetic(temp_buf2[i])) {
						strncat(word_check, &temp_buf2[i], 1);
					}
				}
			} else if (strcmp(type, "imbratisata") == 0) {
				for (i = old_index; i <= strlen(temp_buf2); ++i) {
					if (is_space(temp_buf2[i])) {
						memset(word_check, 0, sizeof(word_check));
					} else if (is_newline(temp_buf2[i])) {
						if (strcmp(word_check, token) == 0) {
							old_index = i;
							if (mode_1_4 == 1) {
								while (newline_flag != 3 &&
								       i <= strlen(temp_buf2)) {
									++i;
									if (is_newline(temp_buf2[i]) ||
									    temp_buf2[i] == '\0') {
										++newline_flag;
									}
								}
								while (!is_alphabetic(temp_buf2[i])) {
									--i;
								}
								letter_to_rhyme = temp_buf2[i];
								if (is_uppercase(letter_to_rhyme)) {
									letter_to_rhyme += case_difference;
								}
								memset(word_check, 0, sizeof(word_check));
								break;
							} else {
								while (newline_flag != 1 &&
								       i <= strlen(temp_buf2)) {
									++i;
									if (is_newline(temp_buf2[i])) {
										++newline_flag;
									}
								}
								while (!is_alphabetic(temp_buf2[i])) {
									--i;
								}
								letter_to_rhyme = temp_buf2[i];
								if (is_uppercase(letter_to_rhyme)) {
									letter_to_rhyme += case_difference;
								}
								memset(word_check, 0, sizeof(word_check));
								while ((!is_newline(temp_buf[i]) &&
								        !is_newline(temp_buf[i + 1])) ||
								       temp_buf2[i] != '\0') {
									++i;
									if (is_newline(temp_buf2[i + 1])) {
										i += 2;
									} else if (temp_buf2[i] == '\0') {
										break;
									}
								}
								old_index = i;
								break;
							}
						}
					} else if (is_alphabetic(temp_buf2[i])) {
						strncat(word_check, &temp_buf2[i], 1);
					}
				}
			} else if (strcmp(type, "incrucisata") == 0) {
				for (i = old_index; i < strlen(temp_buf2); ++i) {
					if (is_space(temp_buf2[i])) {
						memset(word_check, 0, sizeof(word_check));
					} else if (is_newline(temp_buf2[i])) {
						if (strcmp(word_check, token) == 0) {
							while (newline_flag != 2 &&
							       i <= strlen(temp_buf2)) {
								++i;
								if (is_newline(temp_buf2[i]) ||
								    temp_buf2[i] == '\0') {
									++newline_flag;
								}
							}
							while (!is_alphabetic(temp_buf2[i])) {
								--i;
							}
							letter_to_rhyme = temp_buf2[i];
							if (is_uppercase(letter_to_rhyme)) {
								letter_to_rhyme += case_difference;
							}
							if (is_newline(temp_buf2[i + 1]) &&
							    is_newline(temp_buf2[i + 2])) {
								i += 3;
							} else if (temp_buf2[i + 1] == '\0') {
								break;
							} else if (is_newline(temp_buf2[i + 1])) {
								i += 2;
							}
							old_index = i;
							break;
						}
					} else if (is_alphabetic(temp_buf2[i])) {
						strncat(word_check, &temp_buf2[i], 1);
					}
				}
			}
			if (letter_to_rhyme == 0) {
				return;
			}
			// Get all words that end on the letter that rhymes,
			// copy them to rhymes so we can compare them lexicographically.
			if (letter_to_rhyme != token[strlen(token) - 1]) {
				number_of_rhymes = 0;
				for (i = 0; i < length; ++i) {
					if (words[i][strlen(words[i]) - 1] == letter_to_rhyme) {
						strcpy(rhymes[number_of_rhymes], words[i]);
						++number_of_rhymes;
					}
				}
				if (number_of_rhymes == 0) {
					return;
				}
				min_lexic = 0;
				for (i = 0; i < number_of_rhymes; ++i) {
					if (strcmp(rhymes[min_lexic], rhymes[i]) > 0) {
						min_lexic = i;
					}
				}
				// Switch, and update position if needed so it
				// continues from where it left off
				char *pos = strstr(buffer, token);
				if (pos != NULL) {
					size_t new_len = strlen(buffer) - strlen(token) +
					                 strlen(rhymes[min_lexic]);
					char *new_buf = malloc(sizeof(char) * new_len + 1);
					memcpy(new_buf, buffer, pos - buffer);
					memcpy(new_buf + (pos - buffer), rhymes[min_lexic],
					       strlen(rhymes[min_lexic]));
					strcpy(new_buf + (pos - buffer) +
					       strlen(rhymes[min_lexic]),
					       pos + strlen(token));
					memcpy(buffer, new_buf, strlen(new_buf) + 1);
					free(new_buf);
					strcpy(temp_buf2, buffer);
					position += strlen(rhymes[min_lexic]) - strlen(token);
					if (strcmp(type, "imbratisata") == 0) {
						mode_1_4 = mode_1_4 == 1 ? 4 : 1;
						newline_flag = 0;
					}
				}
			}
		}
		token = strtok(NULL, " ,.\n");
	}
	if (temp_buf2 != NULL) {
		free(temp_buf2);
	}
}

void print(char *buffer) {
	printf("%s\n\n", buffer);
}

int main(void) {
	srand(42);
	float converted;

	char *buffer = malloc(sizeof(char) * 100000);
	if (buffer == NULL) {
		return -1;
	}
	char *file = malloc(sizeof(char) * 30);
	if (file == NULL) {
		free(buffer);
		return -1;
	}
	char *command = malloc(sizeof(char) * 30);
	if (command == NULL) {
		free(buffer);
		free(file);
		return -1;
	}
	char *probability = malloc(sizeof(char) * 3);
	if (probability == NULL) {
		free(buffer);
		free(file);
		free(command);
		return -1;
	}
	char *rhyme_type = malloc(sizeof(char) * 15);
	if (rhyme_type == NULL) {
		free(buffer);
		free(file);
		free(command);
		free(probability);
		return -1;
	}

	for (;;) {
		scanf("%s", command);
		if (strcmp(command, "load") == 0) {
			scanf("%s", file);
			load_file(file, buffer);
		} else if (strcmp(command, "print") == 0) {
			print(buffer);
		} else if (strcmp(command, "uppercase") == 0) {
			uppercase(buffer);
		} else if (strcmp(command, "trimming") == 0) {
			trimming(buffer);
		} else if (strcmp(command, "make_it_friendlier") == 0) {
			friendly(buffer);
		} else if (strcmp(command, "make_it_silly") == 0) {
			scanf("%s", probability);
			converted = atof(probability);
			silly(buffer, converted);
		} else if (strcmp(command, "make_it_rhyme") == 0) {
			scanf("%s", rhyme_type);
			rhimy(buffer, rhyme_type);
		} else if (strcmp(command, "quit") == 0) {
			free(buffer);
			free(file);
			free(command);
			free(rhyme_type);
			free(probability);
			return 0;
		}
	}
}