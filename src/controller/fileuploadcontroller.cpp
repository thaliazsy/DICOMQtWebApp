/**
  @file
  @author Stefan Frings
*/

#include "fileuploadcontroller.h"
#include "dicomheader.h"
#include "QDir"
#include <QDebug>
#include <QtXml>

FileUploadController::FileUploadController()
{}

void FileUploadController::readHTML(HttpResponse& response, QByteArray string)
{
    QFile html("D:/TCU/Report/QtWebApp/Demo1/src/controller/html/fileupload.html");
    if(html.open(QIODevice::ReadOnly)) {
        response.setHeader("Content-Type", "text/html; charset=ISO-8859-1");
        response.write(string);
        while(!html.atEnd() && !html.error()){
            QByteArray line= html.readLine();
            response.write(line);
        }
    }
    else {
        response.write("html read failed");
    }
}

void FileUploadController::decodeDCM()
{
    LoadDictionary("D:/1QtWebApp/Demo1/src/controller/html/dd.txt");   // DICOM tags and VR dictionary
    DICOMDataObject DDO;  // DICOM Decode Object
    QDir curdir;

    QString input = curdir.currentPath()+"/DICOM/temp.dcm";
    char *inputDCM= input.toUtf8().data();
    printf("---------------------------------");
    printf(inputDCM);
    printf("---------------------------------\n");
    // DDO.ReadDICOMFileObject(inputDCM, 0, false, true); //otak study1
    DDO.ReadDICOMPart10File(inputDCM); //解開之後送到DDO

    QString output = curdir.currentPath()+"/DICOM/temp.xml";
    char *outputXML= output.toUtf8().data();
    printf("---------------------------------");
    printf(outputXML);
    printf("---------------------------------\n");
    DDO.DecodeRetToXML(outputXML);   //轉XML


    //DDO.ReadDICOMPart10File(str); //解開之後送到DDO
    //char inputDCM[] = "D:\\TCU\\Report\\dicom web\\Study1\\DCMfiles\\MR2S1IM7.dcm";
    //char outputXML[] = "D:\\TCU\\Report\\dicom web\\Study1\\DCMfiles\\123.xml.";

    //send to client
    //使用QT後端繪圖

    //DDO.ReadDICOMFileObject(str,0, true,false);
    //int ReadDICOMFileObject(char * FileName, unsigned int DDOOffset, bool inIsExplicitVR,bool inIsLittleEanian);
    printf("Decode finished\n");
}

void FileUploadController::saveDCM(HttpResponse& response, QTemporaryFile* file, QByteArray name_ext)
{
    QDir curdir;
    QFile myDCM(curdir.currentPath()+"/DICOM/temp.dcm");
    QFileInfo fi(myDCM);
    qDebug()<<"==================================="<< fi.absoluteFilePath();
    if(myDCM.open(QIODevice::WriteOnly))
    {
        while (!file->atEnd() && !file->error())
        {
            QByteArray buffer=file->read(65536);
            myDCM.write(buffer);
        }
        response.write(name_ext);
        response.write(" upload success");
        myDCM.close();
    }
    else
    {
        response.write("upload failed");
    }
}

void FileUploadController::readXML(HttpResponse& response, QString& savepath, QByteArray &filename)
{
    QDir curdir;
    QFile xml(curdir.currentPath()+"/DICOM/temp.xml");
    QDomDocument xmlBOM;

    if(xml.open(QIODevice::ReadOnly)) {
        xmlBOM.setContent(&xml);
        xml.close();

        QDomElement root = xmlBOM.documentElement();
        QDomElement Component=root.firstChild().toElement();

        QString study, series;

        while(!Component.isNull()){
            QString key=Component.attribute("keyword");
            if(key=="SOPInstanceUID"){
                QDomElement Child=Component.firstChild().toElement();
                filename = Child.firstChild().toText().data().toUtf8();
                qDebug() << "=================================== instance: " << filename;
            }
            else if(key=="StudyInstanceUID"){
                QDomElement Child=Component.firstChild().toElement();
                study = Child.firstChild().toText().data();
                qDebug() << "=================================== study: " << study;
            }
            else if(key=="SeriesInstanceUID"){
                QDomElement Child=Component.firstChild().toElement();
                series = Child.firstChild().toText().data();
                qDebug() << "=================================== series: " << series;
            }
            Component = Component.nextSibling().toElement();
        }
        QDir dir, dir1;
        savepath = dir1.currentPath()+"/DICOM/"+study+"/"+series+"/";
        if(!dir.exists(savepath)){
            dir.mkpath(savepath);
        }
        qDebug() << "=================================== savepath: " << savepath<<"/"<<filename<<".xml";
        xml.rename(savepath+"/"+filename+".xml");
    }
    else {
        response.write("xml read failed");
    }
}

void FileUploadController::service(HttpRequest& request, HttpResponse& response)
{


    if (request.getParameter("action")=="123")
    {
        QByteArray uploadpath=request.getParameter("file1");
        QList<QByteArray> splits=uploadpath.split('\\');
        QByteArray name_ext = splits.last();
        QList<QByteArray> extension= name_ext.split('.');
        QByteArray filename = extension.first();
        if(extension.last()!="dcm"){
            readHTML(response, "You are not uploading a DICOM file.\n");
        }
        else {
            QTemporaryFile* file=request.getUploadedFile("file1");
            if (file)
            {
                QDir curdir;
                QString savePath;
                saveDCM(response, file, name_ext);
                decodeDCM();
                readXML(response, savePath, filename);  //read xml and mkdir
                QFile myDCM(curdir.currentPath()+"/DICOM/temp.dcm");
                myDCM.rename(savePath+"/"+filename+".dcm");
            }
            else
            {
                response.write("upload failed");
            }
        }
    }
    else
    {
        readHTML(response, "");
    }
}
