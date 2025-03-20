#include "utils.h"
#include <fstream>
#include <sstream>

#include <QMessageBox>
#include <QByteArray>
#include <QIODevice>
#include <QFile>
#include <QDir>

#include<ZipFile.h>

#include<json.h>

#include "combomessagebox.h"
#include "guithreadslotter.h"
#include "progressmessagebox.h"
#include "mergedefaultworkerthread.h"
#include "mergenondefaultworkerthread.h"

QString Utils::getUser(){
    return USER;
}

void Utils::updateUser(){
#ifdef Q_OS_WIN
    wchar_t username [UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserNameW (username, &username_len);
    USER = QString::fromWCharArray(username);
#elif defined(Q_OS_MAC)
    USER = QString::fromUtf8(getenv("USER"));
#elif defined(Q_OS_LINUX)
    QProcess process;
    process.start("whoami");
    process.waitForFinished(-1);
    QString output = process.readAllStandardOutput();
    USER = output;
#endif
}

Qt::ColorScheme Utils::getSystemTheme(){
    return QGuiApplication::styleHints()->colorScheme();
}

bool Utils::isSquare(QString path){

    unsigned int width,height;
    unsigned char buf[8];

    std::fstream in(path.toStdString());//The png file

    in.seekg(16);//look at the png header

    in.read(reinterpret_cast<char*>(&buf),8);//read the header as a char array

    width=(buf[0]<<24)+(buf[1]<<16)+(buf[2]<<8)+(buf[3]<<0);//width of the png

    height=(buf[4]<<24)+(buf[5]<<16)+(buf[6]<<8)+(buf[7]<<0);//height of the png

    bool result=false;//default is false

    if(width==height)//if it's square
        result=true;

    return result;

}

bool Utils::compareQIcon(const QIcon& icon1, const QIcon& icon2)
{
    QByteArray a1;
    QDataStream stream1(&a1, QIODevice::WriteOnly);
    stream1 << icon1;

    QByteArray a2;
    QDataStream stream2(&a2, QIODevice::WriteOnly);
    stream2 << icon2;

    return a1 == a2;
}

bool Utils::isResourcepack(const QString& path){
    ZipArchive::Ptr pack = ZipFile::Open(path.toStdString());
    ZipArchiveEntry::Ptr mcmeta = pack->GetEntry("pack.mcmeta");
    Json::Value mcmetaJ;
    if(mcmeta == nullptr)return false;
    else {
        *mcmeta->GetDecompressionStream() >> mcmetaJ;
        if(!mcmetaJ["pack"]["pack_format"].isInt())
            return false;
        return true;
    }
}

bool Utils::mergeDefault(const QString& p1, const QString& p2){
    /*
     *  -> prioritize pack 1 nodes in all cases ✅
     *  -> use pack 1's description + {[Merged by MCPackTool]} description ✅
     *  -> use pack1 + pack2's + {[Merged by MCPackTool]} name ✅
     *  -> use pack1 image ✅
     *  -> export to minecraft ✅
     *  -> export as zip ✅
     */

    //open archives
    ZipArchive::Ptr pack1 = ZipFile::Open(p1.toStdString()),
                    pack2 = ZipFile::Open(p2.toStdString());

    //parse mcmetas
    ZipArchiveEntry::Ptr pack1mcmeta = pack1->GetEntry("pack.mcmeta"),
                         pack2mcmeta = pack2->GetEntry("pack.mcmeta");

    Json::Value p1mcmeta, p2mcmeta;

    *pack1mcmeta -> GetDecompressionStream() >> p1mcmeta;
    *pack2mcmeta -> GetDecompressionStream() >> p2mcmeta;

    //get formats/versions

    int p1format = p1mcmeta["pack"]["pack_format"].asInt(),
        p2format = p2mcmeta["pack"]["pack_format"].asInt(),
        selectedFormat = 0;//stores the final pack format to use

    QStringList p1Versions = formatToVersion(p1format),
                p2Versions = formatToVersion(p2format);

    //if both packs contain an invalid pack format
    if(find("INVALID PACK FORMAT", p1Versions) != -1 && find("INVALID PACK FORMAT", p2Versions) != -1){
        QMessageBox::critical(new QLabel, "MCPackTool", "Both packs reference an invalid pack format.", QMessageBox::Ok);
        return false;
    }

    //QString test = "";

    //if game versions conflict, prompt user for game version
    if(p1format != p2format){

        ComboMessageBox *prompt =
            new ComboMessageBox(mergeLists(p1Versions, p2Versions),
                                "  Discrepancy in pack versions:\n  Pack 1: "+stringify(p1Versions)+
                                "\n  Pack 2: "+stringify(p2Versions)+
                                "\n  Which game version should the pack work with?");
        prompt->exec();
        //test = QString::fromStdString(prompt->comboBox()->currentText().toStdString().substr(0,prompt->comboBox()->currentText().size()-1));
        selectedFormat = versionToFormat(QString::fromStdString(prompt->comboBox()->currentText().toStdString().substr(0,prompt->comboBox()->currentText().size()-1)));

    }else selectedFormat = p1format;

    ProgressMessageBox *extraordinary = new ProgressMessageBox("Merging...", "Working...");

    //---------------------- thread stuff
    MergeDefaultWorkerThread *thread = new MergeDefaultWorkerThread();

    thread->setPtrs(pack1, pack2);
    thread->setPaths(p1, p2);

    QWidget::connect(thread, &MergeDefaultWorkerThread::progress   , extraordinary, &ProgressMessageBox::setProgress);
    QWidget::connect(thread, &MergeDefaultWorkerThread::finished   , thread       ,            &QObject::deleteLater);
    QWidget::connect(thread, &MergeDefaultWorkerThread::finished   , extraordinary,       &ProgressMessageBox::close);
    QWidget::connect(thread, &MergeDefaultWorkerThread::doneMerging, &Utils::postMergeNonDefault);

    thread->start();
    //---------------------- end thread stuff

    extraordinary->exec();

    thread->wait(); //wait for merge to finish

    //set pack1 image, format/version, name
    //NAME is the path of the output pack
    //AR is the ZipArchiveEntry::Ptr for the output pack

    ZipArchiveEntry::Ptr oMcmeta = AR->CreateEntry("pack.mcmeta"),
                            oPng = AR->CreateEntry   ("pack.png");

    //set pack png
    // oPng->UseDataDescriptor();
    oPng->SetCompressionStream(*pack1->GetEntry("pack.png")->GetDecompressionStream(),
                               DeflateMethod::Create(),
                               ZipArchiveEntry::CompressionMode::Immediate);

    //process and set mcmeta
    Json::Value oMcmetaJ;

    oMcmetaJ = p1mcmeta;

    oMcmetaJ["pack"]["description"] = (oMcmetaJ["pack"]["description"]).asString()+" {[Merged by MCPackTool]}";
    oMcmetaJ["pack"]["pack_format"] = selectedFormat;

    std::fstream mctmp;
    mctmp.open(".mctmp/pack.mcmeta", std::ios::binary | std::ios::out);
    if(!mctmp){
        QMessageBox::critical(NULL, "MCPackTool", "Failed to create temporary file.\t", QMessageBox::Ok);
        return false;
    }
    mctmp<<oMcmetaJ;
    mctmp.close();
    mctmp.open(".mctmp/pack.mcmeta", std::ios::binary | std::ios::in);
    std::istream* is = new std::istream(mctmp.rdbuf());
    oMcmeta->UseDataDescriptor();
    oMcmeta->SetCompressionStream(*is,
                                  DeflateMethod::Create(),
                                  ZipArchiveEntry::CompressionMode::Immediate);

    ZipFile::SaveAndClose(AR,NAME.toStdString());

    //cleanup
    mctmp.close();
    QFile::remove(".mctmp/pack.mcmeta");
    delete is;

    //copy to minecraft
    if(!cp(NAME, MC_PATH+"/resourcepacks"))
        QMessageBox::warning(NULL, "MCPackTool", "Failed to copy to minecraft.\t", QMessageBox::Ok);

    return true;
}

void Utils::postMergeNonDefault(QString name, ZipArchive::Ptr ar){
    NAME = name;
    AR = ar;
}

// void Utils::conflictPrompter(const QString& p1nameR, const QString& p1assetR, const QString& p1imageR,
//                              const QString& p2nameR, const QString& p2assetR, const QString& p2imageR)
// {
//     ConflictPrompt *pro = new ConflictPrompt(p1nameR, p1assetR, p1imageR, p2nameR, p2assetR, p2imageR);
//     pro->exec();
//     emit instance().conflictResult(pro->result());
// }

bool Utils::mergeNonDefault(const QString& p1, const QString& p2, int modeR,
                            int descriptionR, QString customDescriptionR,
                            int nameR,        QString customNameR,
                            int imageR,       QString customImageR,
                            int toMinecraftR, QString customPathR,
                            int exportAsR)
{

    // qDebug()<<"int modeR: "<<modeR;
    // qDebug()<<"int descriptionR: "<<descriptionR;
    // qDebug()<<"QString customDescriptionR: "<<customDescriptionR;
    // qDebug()<<"int nameR: "<<nameR;
    // qDebug()<<"QString customNameR: "<<customNameR;
    // qDebug()<<"int imageR: "<<imageR;
    // qDebug()<<"QString customImageR: "<<customImageR;
    // qDebug()<<"int toMinecraftR: "<<toMinecraftR;
    // qDebug()<<"QString customPathR: "<<customPathR;
    // qDebug()<<"int exportAsR: "<<exportAsR;

    /*
     * --- EXAMPLE ---
     * int modeR:  1                                                    //prioritize p2 nodes
     * int descriptionR:  3                                             //custom description
     * QString customDescriptionR:  "this is a Custom Description"      //custom description string
     * int nameR:  3                                                    //custom name
     * QString customNameR:  "thisisacustomname"                        //custom name string
     * int imageR:  2                                                   //custom image
     * QString customImageR:  "C:/Users/Peter/Pictures/dead-henry.png"  //custom image path string
     * int toMinecraftR:  1                                             //custom path
     * QString customPathR:  "C:/Users/Peter/Documents/MCPackTool"      //custom export path string
     * int exportAsR:  0                                                //as zip
     *
     * --- MAPPINGS (relative) ---
     * int modeR: 0, 1, 2, 3
     *   -> prioritize p1 nodes, prioritize p2 nodes, leave out both, prompt at every conflict
     * int descriptionR: 0, 1, 2, 3
     *   -> p1 description, p2 description, p1+p2 descriptions, custom description
     * QString customDescriptionR:  "ANY STRING"
     *   -> Don't read unless 'descriptionR' is 3; holds the custom description
     * int nameR: 0, 1, 2, 3
     *   -> p1+p2 filename, p1 filename, p2 filename, custom name
     * QString customNameR:  "ANYSTRINGWITHOUTSPACES"
     *   -> Don't read unless 'nameR' is 3; holds the custom filename
     * int imageR: 0, 1, 2
     *   -> p1 image, p2 image, custom image
     * QString customImageR:  "C:/Users/Peter/Pictures/dead-henry.png"
     *   -> Don't read unless 'imageR' is 2; holds the path to the custom image (already checked if it's square)
     * int toMinecraftR: 0, 1
     *   -> export to minecraft, export to custom folder
     * QString customPathR:  "C:/Users/Peter/Documents/MCPackTool"
     *   -> Don't read unless 'customPathR' is 1; holds the path to the custom output directory
     * int exportAsR: 0, 1
     *   -> export as zipped archive, export as unpacked folder
     */

    //open archives
    ZipArchive::Ptr pack1 = ZipFile::Open(p1.toStdString()),
                    pack2 = ZipFile::Open(p2.toStdString());

    //parse mcmetas
    ZipArchiveEntry::Ptr pack1mcmeta = pack1->GetEntry("pack.mcmeta"),
                         pack2mcmeta = pack2->GetEntry("pack.mcmeta");

    Json::Value p1mcmeta, p2mcmeta;

    *pack1mcmeta -> GetDecompressionStream() >> p1mcmeta;
    *pack2mcmeta -> GetDecompressionStream() >> p2mcmeta;

    //get formats/versions

    int p1format = p1mcmeta["pack"]["pack_format"].asInt(),
        p2format = p2mcmeta["pack"]["pack_format"].asInt(),
        selectedFormat = 0;//stores the final pack format to use

    QStringList p1Versions = formatToVersion(p1format),
        p2Versions = formatToVersion(p2format);

    //if both packs contain an invalid pack format
    if(find("INVALID PACK FORMAT", p1Versions) != -1 && find("INVALID PACK FORMAT", p2Versions) != -1){
        QMessageBox::critical(new QLabel, "MCPackTool", "Both packs reference an invalid pack format.", QMessageBox::Ok);
        return false;
    }

    //if game versions conflict, prompt user for game version
    if(p1format != p2format){

        ComboMessageBox *prompt =
            new ComboMessageBox(mergeLists(p1Versions, p2Versions),
                                "  Discrepancy in pack versions:\n  Pack 1: "+stringify(p1Versions)+
                                    "\n  Pack 2: "+stringify(p2Versions)+
                                    "\n  Which game version should the pack work with?");
        prompt->exec();
        selectedFormat = versionToFormat(QString::fromStdString(prompt->comboBox()->currentText().toStdString().substr(0,prompt->comboBox()->currentText().size()-1)));

    }else selectedFormat = p1format;

    ProgressMessageBox *extraordinary = new ProgressMessageBox("Merging...", "Working...");

    GUIThreadSlotter *conflictPrompterGUI = new GUIThreadSlotter();

    //---------------------- thread stuff
    MergeNonDefaultWorkerThread *thread = new MergeNonDefaultWorkerThread();

    thread->setPtrs(pack1, pack2);
    thread->setPaths(p1, p2);
    thread->setOpts(modeR, descriptionR, nameR, imageR,
                    toMinecraftR, exportAsR,
                    customDescriptionR, customNameR, customImageR, customPathR);
    thread->setGUIThreadSlotter(*conflictPrompterGUI);

    QWidget::connect(thread, &MergeNonDefaultWorkerThread::progress   , extraordinary       ,    &ProgressMessageBox::setProgress);
    QWidget::connect(thread, &MergeNonDefaultWorkerThread::finished   , thread              ,               &QObject::deleteLater);
    QWidget::connect(thread, &MergeNonDefaultWorkerThread::finished   , extraordinary       ,          &ProgressMessageBox::close);
    QWidget::connect(thread, &MergeNonDefaultWorkerThread::conflict   , conflictPrompterGUI , &GUIThreadSlotter::conflictPrompter);
    QWidget::connect(thread, &MergeNonDefaultWorkerThread::doneMerging, &Utils::postMergeNonDefault);

    thread->start();
    //---------------------- end thread stuff

    extraordinary->exec();

    //deal w/ postexec
    Q_UNUSED(selectedFormat);

    return true;
}

int Utils::find(const QString& term, const QStringList& vec){
    int result=-1;//default fail
    for(int i=0;i<vec.size();i++){//iterate
        if(vec[i].toStdString()==term.toStdString())result=i;
    }
    return result;
}

bool Utils::find(const QString& term, std::vector<ZipArchiveEntry::Ptr>& vec){
    for(size_t i=0;i<vec.size();i++)
        if(vec[i]->GetFullName()==term.toStdString())
            return true;
    return false;
}

int Utils::iFind(const QString& term, std::vector<ZipArchiveEntry::Ptr>& vec){
    for(int i=0;i<static_cast<int>(vec.size());i++)
        if(vec[i]->GetFullName()==term.toStdString())
            return i;
    return -1;
}

bool Utils::cp(const QString& i, const QString& o){
    QString inp, out;
#ifdef Q_OS_WIN
    #include <Windows.h>
    const QString program = "copy.bat";
    //convert i and o to windows paths
    inp=convertToWindowsPath(i);
    out=convertToWindowsPath(o);
#elif defined(Q_OS_LINUX)
    const QString program = "cp";
    inp=i;
    out=o;
#endif

    QProcess *process = new QProcess();

    QStringList arguments;
    arguments << inp << out;

    process->start(program, arguments);

    process->waitForFinished(-1);

    if(process->exitCode() == 0)
        return true;
    else
        return false;

}

QString Utils::extractName(const QString& path){
    for(int i=path.length()-1;i>=0;i--)
        if(path[i]=='/' && i!=path.length()-1)
            return QString::fromStdString(path.toStdString().substr(i+1));
    return "";
}

void Utils::mkdir(const QString& path){
    QDir dir;
    dir.mkdir(path);
}

void Utils::rm(const QString& path){
    QFile file(path);
    file.remove();
}

std::vector<std::string> Utils::split(const std::string& s, char delimiter){
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int Utils::easedValue(int currentValue, int targetValue, qreal easingFactor){
    qreal diff = targetValue - currentValue;
    return currentValue + qRound(diff * easingFactor);
}

QString Utils::convertToWindowsPath(const QString& inputPath) {
    QString outputPath;
    for (QChar ch : inputPath) {
        if (ch == '/') {
            outputPath += '\\';
        } else {
            outputPath += ch;
        }
    }
    return outputPath;
}

QString Utils::subSpaces(QString& st){
    for(int i=0;i<st.size();i++){
        if((QChar)st[i]==' '){
            st[i]='-';
        }
    }
    return st;
}

QImage Utils::upscaleImage(const QImage& originalImage, int newWidth, int newHeight) {
    if (!isImage(originalImage) || newWidth <= 0 || newHeight <= 0) return QImage();
    if (newWidth == originalImage.width() && newHeight == originalImage.height()) return originalImage;

    // Create a new image with the upscaled dimensions
    QImage upscaledImage(newWidth, newHeight, QImage::Format_RGB32);

    //create a new image in case there's formatting issues
    QImage originalFormattedImage;

    // Calculate the scale factor for x and y dimensions
    double scaleX = static_cast<double>(newWidth) / originalImage.width();
    double scaleY = static_cast<double>(newHeight) / originalImage.height();

    //check format compatability
    if(originalImage.format() != QImage::Format_RGB32)
        originalFormattedImage = originalImage.convertToFormat(QImage::Format_RGB32);
    else
        originalFormattedImage = originalImage;

    // Iterate over each pixel in the original image
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            // // Map the coordinates in the original image
            int origX = static_cast<int>(x / scaleX);
            int origY = static_cast<int>(y / scaleY);

            // Get the color of the corresponding pixel in the original image
            QRgb pixelColor = originalFormattedImage.pixel(origX, origY);

            // Set the color of the current pixel in the upscaled image
            upscaledImage.setPixel(x, y, pixelColor);
        }
    }
    // qDebug()<<"upscale called";
    return upscaledImage;
}

QImage Utils::downscaleImage(const QImage& originalImage, int newWidth, int newHeight) {
    if (!isImage(originalImage) || newWidth <= 0 || newHeight <= 0) return QImage();
    if (newWidth == originalImage.width() && newHeight == originalImage.height()) return originalImage;

    // Create a new image with the downscaled dimensions
    QImage downscaledImage(newWidth, newHeight, QImage::Format_RGB32);

    //create a new image in case there's formatting issues
    QImage originalFormattedImage;

    // Calculate the scale factor for x and y dimensions
    double scaleX = static_cast<double>(originalImage.width()) / newWidth;
    double scaleY = static_cast<double>(originalImage.height()) / newHeight;

    //check format compatability
    if(originalImage.format() != QImage::Format_RGB32)
        originalFormattedImage = originalImage.convertToFormat(QImage::Format_RGB32);
    else
        originalFormattedImage = originalImage;

    // Iterate over each pixel in the downscaled image
    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            // Map the coordinates in the original image
            int origX = static_cast<int>(x * scaleX);
            int origY = static_cast<int>(y * scaleY);

            // Get the color of the corresponding pixel in the original image
            QRgb pixelColor = originalFormattedImage.pixel(origX, origY);

            // Set the color of the current pixel in the downscaled image
            downscaledImage.setPixel(x, y, pixelColor);
        }
    }
    // qDebug()<<"downscale called";
    return downscaledImage;
}

int Utils::maxValue(int a, int b){
    if(a>b)
        return a;
    else if(b>a)
        return b;
    else
        return a;
}

int Utils::minValue(int a, int b){
    if(a<b)
        return a;
    else if(b<a)
        return b;
    else
        return a;
}

bool Utils::isJson(std::istream *i){
    i->seekg(i->beg);
    Json::Reader reader;
    Json::Value v;
    bool result = reader.parse(*i, v);
    //set the istream back to the begining
    i->seekg(i->beg);
    return result;
}

bool Utils::isImage(std::istream* inputStream) {
    inputStream->seekg(inputStream->beg);
    if (!inputStream || !(*inputStream)) {
        // If the stream is not valid or empty, it cannot be an image
        return false;
    }

    // Read the data from the input stream into a QByteArray
    std::vector<char> buffer(std::istreambuf_iterator<char>(*inputStream), {});
    QByteArray imageData(buffer.data(), buffer.size());

    // Load the image from the QByteArray using QImage
    QImage image;
    bool success = image.loadFromData(imageData);

    //set the istream back to the begining
    inputStream->seekg(inputStream->beg);

    // If the image loading was successful, then it's an image
    return success;
}

bool Utils::isImage(const QImage& i){
    return !i.isNull() && !i.size().isEmpty();
}

void Utils::debaggerlogger(const QString& bleep){
    qDebug()<<bleep;
    std::fstream debugLogFile;
    std::string previous;
    debugLogFile.open("debug.log", std::ios::in);
    if(debugLogFile)
        debugLogFile>>previous;
    //not finished
    return;
}

QString Utils::limitedLength(const QString& st, int len){
    if(len == 0)return "";
    QString result;
    if(st.length() <= len)
        result = st;
    else if(len > 3)
        result = QString::fromStdString(st.toStdString().substr(0,len-4)+"...");
    else
        result = QString::fromStdString(st.toStdString().substr(0,len-1));
    return result;
}
