#include "head/cpphead.h"

using namespace CryptoPP;

QString Crypt::tokenDecrypt(QString word) {
    const std::string key = Config::tokenKey.toStdString(); // 替换为实际的密钥
    const std::string iv = Config::tokenKey.toStdString();   // 替换为实际的IV

    QByteArray wordData = QByteArray::fromBase64(word.toUtf8());
    CBC_Mode<DES>::Decryption decryption;
    decryption.SetKeyWithIV(reinterpret_cast<const byte*>(key.data()), DES::DEFAULT_KEYLENGTH,
        reinterpret_cast<const byte*>(iv.data()));

    std::string decrypted;
    StreamTransformationFilter decryptor(decryption,
        new StringSink(decrypted));

    decryptor.Put(reinterpret_cast<const byte*>(wordData.constData()), wordData.size());
    decryptor.MessageEnd();

    QString decryptedStr = QJsonDocument::fromJson(QString::fromStdString(decrypted).toUtf8()).object().value("value").toString();
    return decryptedStr;
}