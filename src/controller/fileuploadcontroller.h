/**
  @file
  @author Stefan Frings
*/

#ifndef FILEUPLOADCONTROLLER_H
#define FILEUPLOADCONTROLLER_H

#include "httprequest.h"
#include "httpresponse.h"
#include "httprequesthandler.h"

using namespace stefanfrings;

/**
  This controller displays a HTML form for file upload and recieved the file.
*/


class FileUploadController : public HttpRequestHandler {
    Q_OBJECT
    Q_DISABLE_COPY(FileUploadController)
public:

    /** Constructor */
    FileUploadController();

    /** Generates the response */
    void service(HttpRequest& request, HttpResponse& response);

    /** read HTML to show on web */
    void readHTML(HttpResponse& response, QByteArray string);

    /** read XML to show create directories */
    void readXML(HttpResponse& response, QString &path, QByteArray &filename);

    void decodeDCM();
    void saveDCM(HttpResponse& response, QTemporaryFile* file, QByteArray name_ext);

};

#endif // FILEUPLOADCONTROLLER_H
