#include <vector>
#include <string>
#include <regex>
#include <clocale>
#include <unordered_map>

struct Card {
    int user_id;
    std::wstring user_name;
    std::wstring user_surname;
    std::wstring registration_date;
    std::wstring password;
};

class CardsWorkerUTF16 {
private:
    std::wstring pattern_str = L"\"UserID\":([0-9]+),\"UserName\":\"([a-zA-Zа-яА-Я]+)\",\"UserSurname\":\"([a-zA-Zа-яА-Я]+)\",\"RegistrationDate\":\"([0-9]{2}.[0-9]{2}.[0-9]{4})\",\"Password\":\"([0-9a-zA-Z!-~]{8,25})\"}";
    std::unordered_map<int, Card> cards;
    bool PasswordIsValid(const std::wstring& password);
public:
    CardsWorkerUTF16() {std::setlocale(LC_ALL,"");};
    CardsWorkerUTF16(const std::string& pattern_path="D:\\Card-parser\\pattern.txt") {
        std::setlocale(LC_ALL, "");
        std::wstring pattern_str = Read(pattern_path)[0];
    }
    ~CardsWorkerUTF16() = default;

    std::vector<std::wstring> Read(const std::string& path);
    void Write(const std::string& path);
    Card GetCardInfo(std::wstring& card_text);
    void GetCards(const std::string& input_path);
    
    void DelInvalidPasswordCards();
    void DelOldCards(const std::string& year);
};