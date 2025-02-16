#ifndef ALPACADATA_H
#define ALPACADATA_H

#include <QString>


struct AlpacaData {
    QString instruction;
    QString input;
    QString output;

    bool operator==(const AlpacaData& other) const {
        return instruction == other.instruction &&
               input == other.input &&
               output == other.output;
    }
};

#endif // ALPACADATA_H
