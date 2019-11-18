#ifndef FILEDOWNLOAD_H
#define FILEDOWNLOAD_H

#endif

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <QString>

static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
int GetFile(QString outfile, QString url);

