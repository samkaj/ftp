#include "command.h"

enum class Command {
    USER,
    PASS,
    CWD,
    CDUP,
    QUIT,
    PASV,
    PORT,
    TYPE,
    MODE,
    STRU,
    RETR,
    STOR,
    NOOP,
    LIST,
    MKD,
    RMD,
    PWD,
    HELP,
    SYST,
    UNKNOWN
};

Command command_from_string(std::string const& command)
{
    if (command == "USER") return Command::USER;
    if (command == "PASS") return Command::PASS;
    if (command == "CWD")  return Command::CWD;
    if (command == "CDUP") return Command::CDUP;
    if (command == "QUIT") return Command::QUIT;
    if (command == "PASV") return Command::PASV;
    if (command == "PORT") return Command::PORT;
    if (command == "TYPE") return Command::TYPE;
    if (command == "MODE") return Command::MODE;
    if (command == "STRU") return Command::STRU;
    if (command == "RETR") return Command::RETR;
    if (command == "STOR") return Command::STOR;
    if (command == "NOOP") return Command::NOOP;
    if (command == "LIST") return Command::LIST;
    if (command == "MKD")  return Command::MKD;
    if (command == "RMD")  return Command::RMD;
    if (command == "PWD")  return Command::PWD;
    if (command == "HELP") return Command::HELP;
    if (command == "SYST") return Command::SYST;
    return Command::UNKNOWN;
}

std::string command_to_string(Command command)
{
    switch (command)
    {
    case Command::USER: return "USER";
    case Command::PASS: return "PASS";
    case Command::CWD:  return "CWD";
    case Command::CDUP: return "CDUP";
    case Command::QUIT: return "QUIT";
    case Command::PASV: return "PASV";
    case Command::PORT: return "PORT";
    case Command::TYPE: return "TYPE";
    case Command::MODE: return "MODE";
    case Command::STRU: return "STRU";
    case Command::RETR: return "RETR";
    case Command::STOR: return "STOR";
    case Command::NOOP: return "NOOP";
    case Command::LIST: return "LIST";
    case Command::MKD:  return "MKD";
    case Command::RMD:  return "RMD";
    case Command::PWD:  return "PWD";
    case Command::HELP: return "HELP";
    case Command::SYST: return "SYST";
    default:            return "UNKNOWN";
    }
}

