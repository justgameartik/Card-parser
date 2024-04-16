#include "parser.hpp"

#include <fstream>
#include <iostream>
#include <codecvt>

std::vector<std::wstring> CardsWorkerUTF16::Read(const std::string& input_path) {
    std::wifstream input_file(input_path, std::ios::in | std::ios::binary);
    std::vector<std::wstring> text;
    if (input_file.is_open()) {
        input_file.imbue(std::locale(input_file.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));
        std::wstring line;
        while (getline(input_file, line)) {
            text.push_back(line);
        }
    }
    else {
        throw "Input file wasn't opened";
    }
    input_file.close();

    return text;
}

void CardsWorkerUTF16::Write(const std::string& output_path) {
    std::wofstream output_file(output_path, std::ios::out);
    if (output_file.is_open()) {
        output_file << "[\n";
        for (const auto& pair : cards) {
            output_file << "  {\n" <<
                "    \"UserID\": " << pair.second.user_id << ",\n" <<
                "    \"UserName\": \"" << pair.second.user_name << "\",\n" <<
                "    \"UserSurname\": \"" << pair.second.user_surname << "\",\n" <<
                "    \"RegistrationDate\": \"" << pair.second.registration_date << "\",\n" <<
                "    \"Password\": \"" << pair.second.password << "\"\n" <<
                "  }\n";  
        }
        output_file << ']';
    }
    else {
        throw "Output file wasn't opened";
    }
    output_file.close();
}

Card CardsWorkerUTF16::GetCardInfo(std::wstring& card_text) {
    Card cur_card;

    std::wsmatch m;
    std::wregex pattern_reg(pattern_str);
    if (std::regex_match(card_text, m, pattern_reg)) {
        cur_card.user_id = std::stoi(m[1].str());
        cur_card.user_name = m[2].str();
        cur_card.user_surname = m[3].str();
        cur_card.registration_date = m[4].str();
        cur_card.password = m[5].str();
    }
    else {
        throw("No card in line");
    }

    return cur_card;
}

void CardsWorkerUTF16::GetCards(const std::string& input_path) {
    std::vector<std::wstring> text;
    try {
        text = Read(input_path);
    } catch(const char* error_msg) {
        std::cout << "Cards wasn't get: " << error_msg << '\n';
        return;
    }

    std::vector<std::wstring> parsed_text;
    std::wregex pattern_reg(pattern_str);
    for (auto cur_str : text) {
        std::wsregex_iterator cur_match(cur_str.begin(), cur_str.end(), pattern_reg);
        std::wsregex_iterator end_match;
        while (cur_match != end_match) {
            std::wsmatch m = *cur_match;
            parsed_text.push_back(m.str());
            cur_match++;
        }
    }

    for (auto card_text : parsed_text) {
        try {
            auto cur_card = GetCardInfo(card_text);
            cards[cur_card.user_id] = cur_card;
        }
        catch (const char* error_msg) {
            std::wcout << "In line \"" << card_text << "\" card wasn't taken: " << error_msg << '\n';
        }
    }
}

bool CardsWorkerUTF16::PasswordIsValid(const std::wstring& password) {
    bool has_upper_case = std::regex_search(password, std::wregex(L"[A-Z]+"));
    bool has_lower_case = std::regex_search(password, std::wregex(L"[a-z]+"));
    bool has_special_c = std::regex_search(password, std::wregex(L"[!-/:-@\[-`\{-~]+"));
    
    return has_upper_case && has_lower_case && has_special_c;
}

void CardsWorkerUTF16::DelInvalidPasswordCards() {
    for (auto it = cards.begin(); it != cards.end();) {
        if (!PasswordIsValid(it->second.password)) {
            it = cards.erase(it);
        }
        else {
            it++;
        }
    }
}

void CardsWorkerUTF16::DelOldCards(const std::string& year) {
    for (auto it = cards.begin(); it != cards.end();) {
        std::wstring cur_year = it->second.registration_date.substr(
            it->second.registration_date.size()-4,
            it->second.registration_date.size()-1);
        if (std::stoi(cur_year) < std::stoi(year)) {
            it = cards.erase(it);
        }
        else {
            it++;
        }
    }
}