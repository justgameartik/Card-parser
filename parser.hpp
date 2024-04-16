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
    std::wstring pattern_str;
    std::unordered_map<int, Card> cards;
    bool PasswordIsValid(const std::wstring& password);
public:
    CardsWorkerUTF16(const std::string& pattern_path="D:\\Card-parser\\pattern.txt") {
        std::setlocale(LC_ALL, "");
        pattern_str = Read(pattern_path)[0];
    }
    ~CardsWorkerUTF16() = default;

    std::vector<std::wstring> Read(const std::string& input_path);
    void Write(const std::string& output_path);
    Card GetCardInfo(std::wstring& card_text);
    void GetCards(const std::string& input_path);
    
    void DelInvalidPasswordCards();
    void DelOldCards(const std::string& year);
};