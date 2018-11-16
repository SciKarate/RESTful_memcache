#pragma once
#include <cstring> //for "std::memcpy" in set
#include <curl/curl.h> //-lcurl

static void shutdown_server(std::string addr, std::string port)
{
	CURL *curls;
	CURLcode ress;
	curls = curl_easy_init();
	std::string surls;
	surls += addr;
	surls += ":";
	surls += port;
	std::string new_urls = surls + "/shutdown";
	const char* urls = new_urls.c_str();	
	if(curls) //-X POST localhost:18085/shutdown
	{
		curl_easy_setopt(curls, CURLOPT_URL, urls);
		curl_easy_setopt(curls, CURLOPT_POSTFIELDS, "");
		ress = curl_easy_perform(curls);
		if(ress != CURLE_OK)
			{fprintf(stderr, "shutdown failed:\t %s\n", curl_easy_strerror(ress));}
		curl_easy_cleanup(curls);
	}
}