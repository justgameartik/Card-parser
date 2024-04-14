#include "parser.hpp"

int main() {
    CardsWorkerUTF16* card_worker = new CardsWorkerUTF16("D:\\Card-parser\\pattern.txt");
    std::string input_path = "D:\\Card-parser\\dataset_task.txt";
    std::string output_path = "D:\\Card-parser\\result_1.txt";
    card_worker->GetCards(input_path);
    card_worker->Write(output_path);

    card_worker->DelInvalidPasswordCards();
    output_path = "D:\\Card-parser\\result_2.txt";
    card_worker->Write(output_path);
    
    card_worker->DelOldCards("2023");
    output_path = "D:\\Card-parser\\result_3.txt";
    card_worker->Write(output_path);

    delete card_worker;
}