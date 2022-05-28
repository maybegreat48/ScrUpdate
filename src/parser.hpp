#pragma once

namespace upd
{
    // https://stackoverflow.com/a/868894
    class input_parser {
    public:
        input_parser(int& argc, char** argv) 
        {
            for (int i = 1; i < argc; ++i)
                this->tokens.push_back(std::string(argv[i]));
        }
        /// @author iain
        const std::string& get_cmd_option(const std::string& option) const 
        {
            std::vector<std::string>::const_iterator itr;
            itr = std::find(this->tokens.begin(), this->tokens.end(), option);
            if (itr != this->tokens.end() && ++itr != this->tokens.end()) 
                return *itr;
            
            static const std::string empty_string("");
            return empty_string;
        }
        /// @author iain
        bool cmd_option_exists(const std::string& option) const 
        {
            return std::find(this->tokens.begin(), this->tokens.end(), option)
                != this->tokens.end();
        }
    private:
        std::vector <std::string> tokens;
    };
}