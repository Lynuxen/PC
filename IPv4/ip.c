#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_number(unsigned int number, int request)
{
	int i, k = 0;
	unsigned short pr_number = 0;
	unsigned int nr = number;
	for(i = 3; i >= 0; i--)
	{
		if(k == 0)
		{
			pr_number |= nr >> i*8;
			if(request == 10)
			{
				printf("%hu.", pr_number);
			}
			else if(request == 8)
			{
				printf("%o.", pr_number);
			}
			else if(request == 16)
			{
				printf("%X.", pr_number);
			}
			k++;
		}
		else
		{
			pr_number = nr >> i*8;
			pr_number <<= 8;
			pr_number >>= 8;
			if(k == 3)
			{
				if(request == 10)
				{
					printf("%hu", pr_number);
				}
				else if(request == 8)
				{
					printf("%o", pr_number);
				}
				else if(request == 16)
				{
					printf("%X", pr_number);
				}
				k++;
			}
			else
			{
				if(request == 10)
				{
					printf("%hu.", pr_number);
				}
				else if(request == 8)
				{
					printf("%o.", pr_number);
				}
				else if(request == 16)
				{
					printf("%X.", pr_number);
				}
				k++;
			}
		}
	}
}

void print_number_binary(unsigned int number)
{
	int i, dot = 1, last = 3;
	unsigned int temp;
	for(i = sizeof(int)*8-1; i >= 0; i--)
	{
		temp = number >> i;
		if(temp & 1)
		{
			printf("1");

		}
		else
		{
			printf("0");
		}
		if(dot == 8)
		{
			if(last == 0)
			{
				return;
			}
			printf(".");
			last--;
			dot = 0;
		}
		dot++;
	}
}

unsigned int parse_ip_addr(char* number)
{
	char *token;
	unsigned int new_number = 0;
	int i = 3;
	token = strtok(number, ".");

	while(token != NULL)
	{
		new_number |= atoi(token) << 8*i;
		i--;
		token = strtok(NULL, ".");
	}

	return new_number;
}

unsigned int parse_N_addr(char *addr)
{
	char *token, *token2;
	unsigned int new_number = 0;
	int i = 3;

	token = strtok(addr, "/");
	token2 = strtok(token, ".");


	while(token2 != NULL)
	{
		new_number |= atoi(token2) << 8*i;
		i--;
		token2 = strtok(NULL, ".");
	}

	return new_number;
}

int parse_addr_masks(char *addr)
{
	char *token;
	int addr_mask, i = 0;
	token = strtok(addr, "/");
	while(token != NULL)
	{
		if(i == 1)
		{
			addr_mask = atoi(token);
		}
		token = strtok(NULL, "/");
		i++;
	}
	return addr_mask;
}

void task_0(unsigned int ip_1, int msk_2)
{
	printf("-0 ");
	print_number(ip_1, 10);
	printf("/%d\n", msk_2);
}

void task_1(int msk_2)
{
	printf("-1 ");
	unsigned int temp = 0;
	temp = ~temp >> (sizeof(int)*8 - msk_2);
	temp <<= sizeof(int)*8 - msk_2;
	print_number(temp, 10);
	printf("\n");
}

void task_2(unsigned int msk_1)
{
	printf("-2 ");
	print_number(msk_1, 8);
	printf(" ");
	print_number(msk_1, 16);
	printf("\n");
}

void task_3(unsigned int ip_1, int msk_2)
{
	printf("-3 ");
	unsigned int temp = ip_1;
	temp >>= 32 - msk_2;
	temp <<= 32 - msk_2;
	print_number(temp, 10);
	printf("\n");
}

void task_4(unsigned int ip_1, int msk_2)
{
	printf("-4 ");
	unsigned int temp = ip_1;
	unsigned int broad = 0;
	broad = ~(broad & 0);
	broad <<= msk_2;
	broad >>= msk_2;
	temp >>= 32 - msk_2;
	temp <<= 32 - msk_2;
	temp |= broad;
	print_number(temp, 10);
	printf("\n");
}

void task_5(unsigned int ip_1, unsigned int ip_2, int msk_2)
{
	unsigned int temp1 = ip_1, temp2 = ip_2;
	printf("-5 ");
	temp1 >>= 32 - msk_2;
	temp1 <<= 32 - msk_2;
	temp2 >>= 32 - msk_2;
	temp2 <<= 32 - msk_2;
	if(temp1 == temp2)
	{
		printf("DA\n");
	}
	else
	{
		printf("NU\n");
	}
}

int task_6(unsigned int msk_1)
{
	printf("-6 ");
	int flag = 0, i, count = 0;
	unsigned int temp;
	for(i = sizeof(int)*8-1; i >= 0; i--)
	{
		temp = msk_1 >> i;
		if(!(temp & 1))
		{
			flag = 1;
		}
		else if((temp & 1) && flag == 1)
		{
			printf("NU\n");
			return count;
		}
		else
		{
			count++;
		}
	}
	printf("DA\n");
	return 0;
}

void task_7(unsigned int msk_1, int task6)
{
	printf("-7 ");
	if(task6 == 0)
	{
		print_number(msk_1, 10);
		printf("\n");
	}
	else
	{
		msk_1 >>= 32-task6;
		msk_1 <<= 32-task6;
		print_number(msk_1, 10);
		printf("\n");
	}
}

void task_8(unsigned int ip_1)
{
	printf("-8 ");
	print_number_binary(ip_1);
	printf("\n");
}

void task_9(unsigned int ip_2, unsigned int *par_arr, int *addr_masks, int size)
{
	printf("-9");

	int i;

	unsigned int temp = 0;
	for(i = 0; i < size; i++)
	{
		par_arr[i] >>= sizeof(int)*8 - addr_masks[i];
		par_arr[i] <<= sizeof(int)*8 - addr_masks[i];
		temp = ip_2 >> (sizeof(int)*8 - addr_masks[i]);
		temp <<= sizeof(int)*8 - addr_masks[i];
		if((temp ^ par_arr[i]) == 0)
		{
			printf(" %d", i);
		}
	}
	printf("\n");
}

int main(void)
{
	int number_of_sets, i, k, MSK_2, N, *addr_masks, task6, current_set = 1;
	unsigned int MSK_1, IP_1, IP_2, *parsed_array_of_addr;
	char cMSK_1[20], cIP_1[20], cIP_2[20];
	char **array_of_addr;
	

	scanf("%d", &number_of_sets);

	for(i = 0; i < number_of_sets; i++)
	{
		scanf("%s %d %s %s %d", cMSK_1, &MSK_2, cIP_1, cIP_2, &N);
		MSK_1 = parse_ip_addr(cMSK_1);
		IP_1 = parse_ip_addr(cIP_1);
		IP_2 = parse_ip_addr(cIP_2);
		parsed_array_of_addr = malloc(sizeof(unsigned int) * N);
		addr_masks = malloc(sizeof(int) * N);
		array_of_addr = malloc(sizeof(char*) * N);
		for(k = 0; k < N; k++)
		{
			array_of_addr[k] = malloc(sizeof(char) * 20);
			scanf("%s", array_of_addr[k]);
			addr_masks[k] = parse_addr_masks(array_of_addr[k]);
			parsed_array_of_addr[k] = parse_N_addr(array_of_addr[k]);
		}
		printf("%d\n", current_set);
		current_set++;
		task_0(IP_1, MSK_2);
		task_1(MSK_2);
		task_2(MSK_1);
		task_3(IP_1, MSK_2);
		task_4(IP_1, MSK_2);
		task_5(IP_1, IP_2, MSK_2);
		task6 = task_6(MSK_1);
		task_7(MSK_1, task6);
		task_8(IP_1);
		task_9(IP_2, parsed_array_of_addr, addr_masks, N);
		free(parsed_array_of_addr);
		for(k = 0; k < N; k++)
		{
			free(array_of_addr[k]);
		}
		free(addr_masks);
		free(array_of_addr);
	}
	
	return 0;
}