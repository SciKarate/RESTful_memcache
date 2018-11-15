#pragma once
#include <cstring> //for "std::memcpy" in set
#include <curl/curl.h> //-lcurl

static void shutdown_server(std::string addr, std::string port)
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	std::string surl;
	surl += addr;
	surl += ":";
	surl += port;
	std::string new_url = surl + "/shutdown";
	const char* url = new_url.c_str();	
	if(curl) //-X POST localhost:18085/shutdown
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			{fprintf(stderr, "shutdown failed:\t %s\n", curl_easy_strerror(res));}
		curl_easy_cleanup(curl);
	}
}