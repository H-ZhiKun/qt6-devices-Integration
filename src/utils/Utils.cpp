#include "Utils.h"
#include "ConcurrentTaskQueue.h"
#include "Logger.h"

std::string Utils::base64Encode(const unsigned char *str, int bytes)
{
    std::string _base64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int num = 0, bin = 0;
    std::string _encode_result;
    const unsigned char *current;
    current = str;
    while (bytes > 2)
    {
        _encode_result += _base64_table[current[0] >> 2];
        _encode_result += _base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
        _encode_result += _base64_table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
        _encode_result += _base64_table[current[2] & 0x3f];

        current += 3;
        bytes -= 3;
    }
    if (bytes > 0)
    {
        _encode_result += _base64_table[current[0] >> 2];
        if (bytes % 3 == 1)
        {
            _encode_result += _base64_table[(current[0] & 0x03) << 4];
            _encode_result += "==";
        }
        else if (bytes % 3 == 2)
        {
            _encode_result += _base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
            _encode_result += _base64_table[(current[1] & 0x0f) << 2];
            _encode_result += "=";
        }
    }
    return _encode_result;
}

std::string Utils::base64Decode(const char *str, int length)
{
    char base64_pad = '=';
    const char DecodeTable[] = {
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -2, -2, -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 62, -2, -2, -2, 63, 52, 53, 54, 55,
        56, 57, 58, 59, 60, 61, -2, -2, -2, -2, -2, -2, -2, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -2, -2, -2, -2, -2, -2, 26, 27, 28, 29, 30, 31, 32,
        33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2};
    int bin = 0, i = 0, pos = 0;
    std::string _decode_result;
    const char *current = str;
    char ch;
    while ((ch = *current++) != '\0' && length-- > 0)
    {
        if (ch == base64_pad)
        { // 当前一个字符是“=”号
            /*
            先说明一个概念：在解码时，4个字符为一组进行一轮字符匹配。
            两个条件：
                1、如果某一轮匹配的第二个是“=”且第三个字符不是“=”，说明这个带解析字符串不合法，直接返回空
                2、如果当前“=”不是第二个字符，且后面的字符只包含空白符，则说明这个这个条件合法，可以继续。
            */
            if (*current != '=' && (i % 4) == 1)
            {
                return NULL;
            }
            continue;
        }
        ch = DecodeTable[ch];
        // 这个很重要，用来过滤所有不合法的字符
        if (ch < 0)
        { /* a space or some other separator character, we simply skip over */
            continue;
        }
        switch (i % 4)
        {
        case 0:
            bin = ch << 2;
            break;
        case 1:
            bin |= ch >> 4;
            _decode_result += bin;
            bin = (ch & 0x0f) << 4;
            break;
        case 2:
            bin |= ch >> 2;
            _decode_result += bin;
            bin = (ch & 0x03) << 6;
            break;
        case 3:
            bin |= ch;
            _decode_result += bin;
            break;
        }
        i++;
    }
    return _decode_result;
}

Json::Value Utils::stringToJson(const std::string &jsString)
{
    Json::Value root;
    JSONCPP_STRING errs;
    Json::CharReaderBuilder readerBuilder;
    std::unique_ptr<Json::CharReader> const jsonReader(readerBuilder.newCharReader());
    jsonReader->parse(jsString.c_str(), jsString.c_str() + jsString.length(), &root, &errs);
    return root;
}

std::string Utils::jsonToString(const Json::Value &jsVal)
{
    if (jsVal.isNull())
        return {};
    static Json::Value def = []() {
        Json::Value def;
        Json::StreamWriterBuilder::setDefaults(&def);
        def["emitUTF8"] = true;
        return def;
    }();

    std::ostringstream stream;
    Json::StreamWriterBuilder stream_builder;
    stream_builder.settings_ = def; // Config emitUTF8
    std::unique_ptr<Json::StreamWriter> writer(stream_builder.newStreamWriter());
    writer->write(jsVal, &stream);
    return stream.str();
}

Json::Value Utils::readJsonFile(const std::string &filePath)
{
    Json::Value jsVal;
    std::ifstream is(filePath);
    if (!is.is_open())
        return jsVal;
    std::ostringstream osBuf;
    char ch;
    while (osBuf && is.get(ch))
        osBuf.put(ch);
    jsVal = stringToJson(osBuf.str());
    is.close();
    return jsVal;
}

bool Utils::writeJsonFile(const std::string &filePath, const Json::Value &jsVal)
{
    std::ofstream outfile(filePath);
    if (!outfile.is_open())
        return false;
    std::string jsString = jsonToString(jsVal);
    outfile << jsString;
    outfile.close();
    return true;
}

std::vector<std::filesystem::path> Utils::findAllFiles(const std::filesystem::path &rootPath)
{
    std::vector<std::filesystem::path> results;
    for (const auto &entry : std::filesystem::directory_iterator(rootPath))
    {
        const std::filesystem::path &path = entry.path();

        if (std::filesystem::is_directory(path))
        {
            // findAllFiles(path); // 递归遍历子目录
        }
        else if (std::filesystem::is_regular_file(path))
        {
            results.emplace_back(path); // 输出文件路径
        }
    }
    return results;
}

bool Utils::removeOutdatedFiles(const std::string &currentTime, const std::string &path, int offsetDay)
{
    std::string lastTime = getTimeOffset(offsetDay * 24 * 60 * 60, currentTime);
    std::chrono::system_clock::time_point lastPoint = stringToTimePoint(lastTime);
    auto files = findAllFiles(path);
    for (auto item : files)
    {
        std::filesystem::file_time_type modificationTime = std::filesystem::last_write_time(item);
        // 将文件修改时间转换为 time_point
        std::chrono::system_clock::time_point timePoint =
            std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                modificationTime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());

        if (timePoint < lastPoint)
        {
            std::filesystem::remove(item);
        }
    }
    return false;
}

void Utils::asyncTask(std::function<void(void)> &&task)
{
    ConcurrentTaskQueue::getInstance().runTaskInQueue(task);
}

void Utils::appExit(int exitCode)
{
    std::terminate();
}

QImage Utils::matToQImage(const cv::Mat &matData)
{
    QImage img;
    if (matData.empty())
    {
        return img;
    }
    // 将cv::Mat转换为QImage
    if (matData.type() == CV_8UC1)
    {
        img =
            QImage(matData.data, matData.cols, matData.rows, static_cast<int>(matData.step), QImage::Format_Grayscale8);
    }
    else if (matData.type() == CV_8UC3)
    {
        img = QImage(matData.data, matData.cols, matData.rows, static_cast<int>(matData.step), QImage::Format_RGB888);
        img = img.rgbSwapped();
    }
    // cv::mat 是局部变量QImage构造使用的mat.data指针在函数出栈后销毁，导致img引用无效地址
    // 故需要传递img深拷贝到外部存储。

    return img.copy();
}

std::string Utils::makeResponse(bool bOk, Json::Value &&details, std::string &&description)
{
    Json::Value jsResp;
    jsResp["ok"] = bOk;
    jsResp["details"] = std::move(details);
    jsResp["description"] = std::move(description);
    return jsonToString(jsResp);
}

std::chrono::system_clock::time_point Utils::stringToTimePoint(const std::string &timeString)
{
    std::tm tm = {};
    std::istringstream ss(timeString);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    std::time_t time = std::mktime(&tm);
    return std::chrono::system_clock::from_time_t(time);
}

std::string Utils::getCurrentTime(bool hasMillisecond)
{
    // 获取当前系统时间的时间点
    std::string curTime;
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    // 将时间点转换为时间值
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // 格式化时间值为字符串
    std::stringstream ss;
    ss << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S");
    curTime = ss.str();
    if (hasMillisecond)
    {
        curTime += std::string(".") +
                   std::to_string(
                       std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000);
    }
    // 返回格式化后的字符串
    return curTime;
}

void Utils::getCurrentTime(std::string &year, std::string &month, std::string &day, std::string &hour,
                           std::string &minute, std::string &second)
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    // 将时间点转换为时间值
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm *localTime = std::localtime(&currentTime);
    int nYear = localTime->tm_year + 1900; // 年份需要加上1900
    int nMonth = localTime->tm_mon + 1;    // 月份从0开始，需要加1
    int nDay = localTime->tm_mday;
    int nHour = localTime->tm_hour;
    int nMinute = localTime->tm_min;
    int nSecond = localTime->tm_sec;
    // 将各个时间部分转换为字符串
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << nYear;
    year = ss.str();
    ss.str(""); // 清空stringstream
    ss << std::setw(2) << std::setfill('0') << nMonth;
    month = ss.str();
    ss.str("");
    ss << std::setw(2) << std::setfill('0') << nDay;
    day = ss.str();
    ss.str("");

    ss << std::setw(2) << std::setfill('0') << nHour;
    hour = ss.str();
    ss.str("");

    ss << std::setw(2) << std::setfill('0') << nMinute;
    minute = ss.str();
    ss.str("");

    ss << std::setw(2) << std::setfill('0') << nSecond;
    second = ss.str();
}

std::string Utils::getTimeOffset(int seconds, const std::string &currentTime)
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::seconds offset(seconds);
    std::chrono::system_clock::time_point timePoint = now + offset;

    std::time_t resultTime = std::chrono::system_clock::to_time_t(timePoint);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&resultTime), "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

std::string Utils::imgToBase64(const std::string &filePath)
{
    QImage image(filePath.c_str());
    return imgToBase64(image);
}

std::string Utils::imgToBase64(const QImage &img)
{
    if (img.isNull())
        return {};
    QByteArray imageData;
    QBuffer buffer(&imageData);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, "JPEG"); // 保存为 JPEG 格式，也可以选择其他格式
    buffer.close();
    return imageData.toBase64().data();
}

std::string Utils::encrytByAES(const std::string &plain)
{
    std::string key("kungegeinijiami");
    std::string iv("danzongshusihua");
    std::string cipher;
    try
    {
        CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption e;
        e.SetKeyWithIV((CryptoPP::byte *)key.c_str(), 16, (CryptoPP::byte *)iv.c_str());

        // The StreamTransformationFilter removes
        //  padding as required.
        CryptoPP::StringSource s(
            plain, true,
            new CryptoPP::StreamTransformationFilter(e,
                                                     new CryptoPP::StringSink(cipher)) // StreamTransformationFilter
        );                                                                             // StringSource
    }
    catch (const CryptoPP::Exception &e)
    {
        LogError(e.what());
    }

    // Pretty print
    std::string encoded;
    CryptoPP::StringSource(cipher, true,
                           new CryptoPP::Base64Encoder(new CryptoPP::StringSink(encoded)) // HexEncoder
    );                                                                                    // StringSource
    return encoded;
}

std::string Utils::decrytByAES(const std::string &encode)
{
    std::string key("kungegeinijiami");
    std::string iv("danzongshusihua");
    std::string encodeByte;
    CryptoPP::StringSource(encode, true, new CryptoPP::Base64Decoder(new CryptoPP::StringSink(encodeByte)));

    std::string recovered;
    try
    {
        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption d;
        d.SetKeyWithIV((CryptoPP::byte *)key.c_str(), 16, (CryptoPP::byte *)iv.c_str());

        // The StreamTransformationFilter removes
        //  padding as required.
        CryptoPP::StringSource s(
            encodeByte, true,
            new CryptoPP::StreamTransformationFilter(d,
                                                     new CryptoPP::StringSink(recovered)) // StreamTransformationFilter
        );                                                                                // StringSource
    }
    catch (const CryptoPP::Exception &e)
    {
        LogError(e.what());
    }

    return recovered;
}

cv::Mat Utils::qImageToMat(QImage &qim)
{
    cv::Mat mat;
    switch (qim.format())
    {
    case QImage::Format_RGB888:
        mat = cv::Mat(qim.height(), qim.width(), CV_8UC3, (void *)qim.constBits(), qim.bytesPerLine()).clone();
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR); // 如果需要，将RGB转换回BGR
        break;
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(qim.height(), qim.width(), CV_8UC4, (void *)qim.constBits(), qim.bytesPerLine()).clone();
        cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR); // 将ARGB转换为BGR
        break;
    }
    return mat;
}
