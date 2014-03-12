#ifndef FOX_RIVER_WORD_ASSEMBLER_H
#define FOX_RIVER_WORD_ASSEMBLER_H

#include <CppJieba/MPSegment.hpp>

namespace FoxRiver
{
    class WordAssembler
    {
        private:
            MPSegment _segment;
        public:
            WordAssembler(const string& dictPath): _segment(dictPath)
            {}
            ~WordAssembler()
            {}
        public:
            bool run(const string& sentence, string& res) const
            {
                vector<string> words;
                return _segment(sentenceres, words);
            }
    };
}

#endif
