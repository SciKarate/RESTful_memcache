#pragma once
#include <cstring> //for "std::memcpy" in set
#include <curl/curl.h> //-lcurl

static size_t writer(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

static std::string ret_header (std::string addr, std::string port)
{
	CURL *curl;
	CURLcode res;
	std::string surl;
	surl += addr;
	surl += ":";
	surl += port;
	std::string key = "howdy"; 
	curl = curl_easy_init();
	std::string outstring;
	std::string new_url = surl + "/key/" + key;
	const char* url = new_url.c_str();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
		//curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outstring);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK)
			{
				fprintf(stderr, "Header request failed:\t %s\n", curl_easy_strerror(res));
				return NULL;
			}
		curl_easy_cleanup(curl);
	}
	return outstring;
}