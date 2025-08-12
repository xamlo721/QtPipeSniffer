#ifndef ENUMCONNECTIONSTATUS_H
#define ENUMCONNECTIONSTATUS_H

#include <QString>

class EnumConnectionStatus {
    public:
        enum Value : qint32 {

            Connected,

            Disconnected,

            Unknown
        };

        EnumConnectionStatus() = default;
        constexpr EnumConnectionStatus(Value type) : value(type) {}
        constexpr EnumConnectionStatus(qint8 type) : value((Value)type) {}
        //allow switch and ><=. switch(EnumObjectStatus) {}
        constexpr operator Value() const {return value; }

        //disable if(EnumObjectStatus)-else
        explicit operator bool() const = delete;


        void fromString(QString name) {
            if (name  == "Connected")      this->value = Connected;
            else if (name  == "Disconnected")     this->value = Disconnected;
            else                            this->value = Unknown;
        }

        QString toString() const {
            switch (this->value) {
                case Connected:    return "Connected";
                case Disconnected:        return "Disconnected";
                default:
                    return "Unknown";
            }
        }

        private:
            Value value;

};

#endif // ENUMCONNECTIONSTATUS_H
