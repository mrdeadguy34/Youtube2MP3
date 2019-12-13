/* File source code pretty much copied from:
 * https://curl.haxx.se/libcurl/c/url2file.html
*/

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <QString>
#include <QDebug>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int GetFile(QString outfile, QString url)
{ // Simple libcUrl function to download files from the web
    CURL *curl;
    FILE *fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        QByteArray outfileBArray = outfile.toUtf8();
        const char* outfileCStr = outfileBArray.constData();
        QByteArray urlBArray = url.toUtf8();
        const char* urlCStr = urlBArray.constData();
        errno_t err;
        err = fopen_s(&fp, outfileCStr, "wb");
        if (err == 0) {
            qDebug() << "opened";
        } else {
            qDebug() << "error";
        }
        curl_easy_setopt(curl, CURLOPT_URL, urlCStr);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if(fp != NULL) {
            fclose(fp);
        }
    }
    else {
        return 0;
    }
    return 1;
}
