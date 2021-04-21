#include "Logger.hpp"

Logger::Logger()
{
}
Logger::~Logger()
{
    if (Worker.joinable())
        Worker.join();
    std::cout << "Logger is stopped" << std::endl;
}
void Logger::Start()
{
    Worker = std::thread([]() {
        while (LogPool::Pool().Active)
        {
            auto T = LogPool::Pool().popTask();
            if (T)
            {
                std::ofstream File(RESULT_FILE, std::ios::binary | std::ios::out | std::ios::app);
                std::vector<unsigned char> Data = T->toMsg();
                LongBuf Size;
                Size.Id = Data.size();

                File.write((char*)Size.Buf, 8);
                File.write((char*)Data.data(), Size.Id);

                delete T;
                File.flush();
                File.close();
            }
        }
    });
}
void Logger::Stop()
{
    LogPool::Pool().StopService();
    if (Worker.joinable())
        Worker.join();
    std::cout << "Stop logger" << std::endl;
}