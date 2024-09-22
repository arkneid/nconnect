// includes
#include "functions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
/***********************/

void write_file_history(char* net_ssid, char* net_pass, char* w_card, const char* file_name);

void help()
{
	printf("Usage: nconnect [ARGS]\n"
	"Connect to wireless network provided.\n"
	"\n"
	"ARGS:\n"
	"  -s\t\tSSID of the network\n"
	"  -p\t\tPassword of the network\n"
	"  -w\t\tWireless card name\n"
	"  -m\t\tMode of connection\n"
	"  -H\t\tHistory of connections. This argument just can be used with -s argument\n"
	"  -h, --help\tDisplay this help and exit\n"
	"\n"
	"Examples:\n"
	" Normal mode:\n"
	"  nconnect -s Dummy_Network -p nEtWorkPass -w wlan1\n"
	"\n Auth Portal mode:\n"
	"  nconnect -m portal -s Dummy_Network -w wlan1\n"
	"\n History:\n"
	"  nconnect -H -s Dummy_Network -w wlan1\n"
	"\n"
	);
}

int check_args(int nargs, char** args, char* str_compare)
{
	int present;
	int i;
	
	i = 0;
	present = 0;
	while (i < nargs)
	{
		if (nc_strcmp(args[i], str_compare) == 0)
		{
			present = 1;
			break;
		}
		i++;
	}
	
	return present;
}

void file_rename(char* src, const char* dst)
{
	FILE* f_src;
	FILE* f_dst;
	char line[100];

	f_src = fopen(src, "r");
	f_dst = fopen(dst, "w");
	if(f_src != NULL || f_dst != NULL)
	{
		while(fgets(line, 100, f_src))
		{
			fputs(line, f_dst);
		}		
	}
	else
		printf("Something went wrong while renaming the tmp file to the original file");
}

void check_inet_line(const char* orig_file, char* w_card)
{
	char line[100];
	FILE* orig_file_r;
        FILE* f_w;
	bool written = false;
	char n_line[100] = "";

	orig_file_r = fopen(orig_file, "r");
        f_w = fopen("replace_check.tmp", "w");
        if(orig_file_r != NULL || f_w != NULL)
        {
		while(fgets(line, 100, orig_file_r))
		{
			if(strstr(line, w_card) != NULL && strstr(line, "inet") != NULL && strstr(line, "iface") != NULL)
			{
				written = true;
				fputs(line, f_w);
			}
			else if(strstr(line, "wireless-essid") == NULL && strstr(line, "wireless-mode") == NULL && strstr(line, "wpa-ssid") == NULL && strstr(line, "wpa-psk") == NULL)
			{
				fputs(line, f_w);
			}
		}

		if(written == 0)
		{
			strcat(n_line, "\n# Custom nconnect\niface ");
			strcat(n_line, w_card);
			strcat(n_line, " inet dhcp");
			strcat(n_line, "\n");
			fputs(n_line, f_w);
		}
	}
	else
	{
		printf("One of the files was not accessible!");
	}
	fclose(f_w);
        fclose(orig_file_r);
}

void write_file(int nargs, char** args, const char* file_name)
{
	int i;
	char* ssid;
	char* password;
	char* wireless_card;
	char line[100];
	char final_line[100];
	FILE* file_r;
	FILE* file_w;

	i = 0;
	while(i < nargs)
	{
		if(nc_strcmp(args[i], "-s") == 0)
		{
			ssid = nc_strcpy(ssid, args[i+1]);
		}
		else if(nc_strcmp(args[i], "-p") == 0)
		{
			password = nc_strcpy(password, args[i+1]);
		}
		else if(nc_strcmp(args[i], "-w") == 0)
		{
			wireless_card = nc_strcpy(wireless_card, args[i+1]);
		}
		i++;
	}
	printf("SSID: %s\n", ssid);
	printf("PASSWORD: %s\n", password);
	printf("Wireless Card: %s\n", wireless_card);
	
	check_inet_line(file_name, wireless_card);
	
	file_r = fopen("replace_check.tmp", "r");
	file_w = fopen("replace.tmp", "w");
	if(file_r != NULL || file_w != NULL)
	{
		while(fgets(line, 100, file_r))
		{
			if(strstr(line, wireless_card) != NULL && strstr(line, "inet") != NULL && strstr(line, "iface") != NULL)
			{
				strcat(final_line, line);
				strcat(final_line, "\t");
				strcat(final_line, "wpa-ssid ");
				strcat(final_line, ssid);
				strcat(final_line, "\n");
				strcat(final_line, "\t");
				strcat(final_line, "wpa-psk ");
				strcat(final_line, password);
				strcat(final_line, "\n");
				fputs(final_line, file_w);
			}
			else if(strstr(line, "wireless-essid") == NULL && strstr(line, "wireless-mode") == NULL && strstr(line, "wpa-ssid") == NULL && strstr(line, "wpa-psk") == NULL)
			{
				fputs(line, file_w);
			}
		}
	}
	else
	{
		printf("One of the files was not accessible!");
	}
	fclose(file_w);
	fclose(file_r);
	remove(file_name);
	file_rename("replace.tmp", file_name);
	remove("replace.tmp");
	remove("replace_check.tmp");
	free(ssid);
	free(password);
	free(wireless_card);
}

void write_file_portal(int nargs, char** args, const char* file_name)
{
	int i;
	char* ssid;
	char* conn_mode;
	char* wireless_card;
	char line[100];
	char final_line[100];
	FILE* file_r;
	FILE* file_w;

	i = 0;
	while(i < nargs)
	{
		if(nc_strcmp(args[i], "-s") == 0)
		{
			ssid = nc_strcpy(ssid, args[i+1]);
		}
		else if(nc_strcmp(args[i], "-m") == 0)
		{
			conn_mode = nc_strcpy(conn_mode, args[i+1]);
		}
		else if(nc_strcmp(args[i], "-w") == 0)
		{
			wireless_card = nc_strcpy(wireless_card, args[i+1]);
		}
		i++;
	}
	printf("SSID: %s\n", ssid);
	printf("MODE: %s\n", conn_mode);
	printf("Wireless Card: %s\n", wireless_card);
	
	check_inet_line(file_name, wireless_card);
	
	file_r = fopen("replace_check.tmp", "r");
	file_w = fopen("replace.tmp", "w");
	if(file_r != NULL || file_w != NULL)
	{
		while(fgets(line, 100, file_r))
		{
			if(strstr(line, wireless_card) != NULL && strstr(line, "inet") != NULL && strstr(line, "iface") != NULL)
			{
				strcat(final_line, line);
				strcat(final_line, "\t");
				strcat(final_line, "wireless-essid ");
				strcat(final_line, ssid);
				strcat(final_line, "\n");
				strcat(final_line, "\t");
				strcat(final_line, "wireless-mode ");
				strcat(final_line, "managed");
				strcat(final_line, "\n");
				fputs(final_line, file_w);
			}
			else if(strstr(line, "wireless-essid") == NULL && strstr(line, "wireless-mode") == NULL && strstr(line, "wpa-ssid") == NULL && strstr(line, "wpa-psk") == NULL)
			{
				fputs(line, file_w);
			}
		}
	}
	else
	{
		printf("One of the files was not accessible!");
	}
	fclose(file_w);
	fclose(file_r);
	remove(file_name);
	file_rename("replace.tmp", file_name);
	remove("replace.tmp");
	remove("replace_check.tmp");
	free(ssid);
	free(conn_mode);
	free(wireless_card);
}


void connect_history(char* network_name, char* net_card, const char* int_file)
{
	char* HOME_FOLDER = getenv("HOME");
	const char* history_file = strcat(HOME_FOLDER, "/.local/share/nconnect/history.log");
	FILE* file_r;
	char line[100];
	char* net_pass;
	char* token;
	int i;
	char final_net_pass[20];

	
	file_r = fopen(history_file, "r");
	if(file_r != NULL)
	{
		while(fgets(line, 100, file_r))
		{
			if(strstr(line, network_name) != NULL)
			{
				token = strtok(line, " | ");
				while (token != NULL)
				{
					if (strstr(token, network_name) == NULL)
					{
						net_pass = token;
					}
        				token = strtok(NULL, " | ");
    				}
			}
		}
	}
	i = 0;
	while(net_pass[i] != '\0')
	{
		if (net_pass[i] != ' ' && net_pass[i] != '\t' && net_pass[i] != '\n' && net_pass[i] != '\r')
		{
            		final_net_pass[i] = net_pass[i];
        	}	
		i++;
	}
	final_net_pass[i] = '\0';
	write_file_history(network_name, final_net_pass, net_card, int_file);
}

void write_file_history(char* net_ssid, char* net_pass, char* w_card, const char* file_name)
{
	char* ssid = "";
	char* password = "";
	char* wireless_card = "";
	char line[100];
	char final_line[100];
	FILE* file_r;
	FILE* file_w;

	ssid = nc_strcpy(ssid, net_ssid);
	password = nc_strcpy(password, net_pass);
	wireless_card = nc_strcpy(wireless_card, w_card);

	printf("SSID: %s\n", ssid);
	printf("PASSWORD: %s\n", password);
	printf("Wireless Card: %s\n", wireless_card);
	
	check_inet_line(file_name, wireless_card);
	
	file_r = fopen("replace_check.tmp", "r");
	file_w = fopen("replace.tmp", "w");
	if(file_r != NULL || file_w != NULL)
	{
		while(fgets(line, 100, file_r))
		{
			if(strstr(line, wireless_card) != NULL && strstr(line, "inet") != NULL && strstr(line, "iface") != NULL)
			{
				strcat(final_line, line);
				strcat(final_line, "\t");
				strcat(final_line, "wpa-ssid ");
				strcat(final_line, ssid);
				strcat(final_line, "\n");
				strcat(final_line, "\t");
				strcat(final_line, "wpa-psk ");
				strcat(final_line, password);
				strcat(final_line, "\n");
				fputs(final_line, file_w);
			}
			else if(strstr(line, "wireless-essid") == NULL && strstr(line, "wireless-mode") == NULL && strstr(line, "wpa-ssid") == NULL && strstr(line, "wpa-psk") == NULL)
			{
				fputs(line, file_w);
			}
		}
	}
	else
	{
		printf("One of the files was not accessible!");
	}
	fclose(file_w);
	fclose(file_r);
	remove(file_name);
	file_rename("replace.tmp", file_name);
	remove("replace.tmp");
	remove("replace_check.tmp");
}
