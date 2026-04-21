#ifndef SRC_HPP
#define SRC_HPP

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>

class BaseJudger {
public:
    BaseJudger (size_t time_limit , size_t memory_limit ,
                const char *answer) : time_limit_(time_limit), memory_limit_(memory_limit), score_(0) {
        answer_ = new char[strlen(answer) + 1];
        strcpy(answer_, answer);
    }

    virtual void Submit (size_t time , size_t memory , const char *output) = 0;

    size_t GetScore () const { return score_; }

    virtual ~BaseJudger () {
        delete[] answer_;
    };

protected:
    char *answer_;
    const size_t time_limit_;
    const size_t memory_limit_;
    size_t score_;

    virtual bool CheckAnswer (const char *output) const {
        // the output must equal to the answer
        return strcmp(answer_ , output) == 0;
    }
};

class ICPCJudger : public BaseJudger {
public:
    ICPCJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer) {}

    virtual void Submit(size_t time, size_t memory, const char *output) override {
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            if (100 > score_) score_ = 100;
        }
    }
};

class OIJudger : public BaseJudger {
private:
    int submit_count_;
public:
    OIJudger(size_t time_limit, size_t memory_limit, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer), submit_count_(0) {}

    virtual void Submit(size_t time, size_t memory, const char *output) override {
        submit_count_++;
        if (submit_count_ > 1) {
            score_ = 0;
            return;
        }
        if (time <= time_limit_ && memory <= memory_limit_ && CheckAnswer(output)) {
            score_ = 100;
        } else {
            score_ = 0;
        }
    }

protected:
    virtual bool CheckAnswer(const char *output) const override {
        const char *p1 = answer_;
        const char *p2 = output;

        while (*p1 != '\0' && *p2 != '\0') {
            const char *line_end1 = p1;
            while (*line_end1 != '\n' && *line_end1 != '\0') line_end1++;
            const char *trim_end1 = line_end1;
            while (trim_end1 > p1 && *(trim_end1 - 1) == ' ') trim_end1--;

            const char *line_end2 = p2;
            while (*line_end2 != '\n' && *line_end2 != '\0') line_end2++;
            const char *trim_end2 = line_end2;
            while (trim_end2 > p2 && *(trim_end2 - 1) == ' ') trim_end2--;

            size_t len1 = trim_end1 - p1;
            size_t len2 = trim_end2 - p2;

            if (len1 != len2 || strncmp(p1, p2, len1) != 0) return false;

            p1 = line_end1;
            if (*p1 == '\n') p1++;
            p2 = line_end2;
            if (*p2 == '\n') p2++;
        }
        while (*p1 == '\n' || *p1 == ' ') p1++;
        while (*p2 == '\n' || *p2 == ' ') p2++;

        return *p1 == *p2;
    }
};

class SpacialJudger : public BaseJudger {
private:
    size_t full_score_time_;
    size_t full_score_memory_;
public:
    SpacialJudger(size_t time_limit, size_t memory_limit, size_t full_score_time, size_t full_score_memory, const char *answer)
        : BaseJudger(time_limit, memory_limit, answer), full_score_time_(full_score_time), full_score_memory_(full_score_memory) {}

    virtual void Submit(size_t time, size_t memory, const char *output) override {
        if (!CheckAnswer(output)) return;

        size_t time_score = 0;
        if (time <= full_score_time_) {
            time_score = 100;
        } else if (time >= time_limit_) {
            time_score = 0;
        } else {
            time_score = (size_t)floor(100.0 * (double)(time_limit_ - time) / (double)(time_limit_ - full_score_time_));
        }

        size_t memory_score = 0;
        if (memory <= full_score_memory_) {
            memory_score = 100;
        } else if (memory >= memory_limit_) {
            memory_score = 0;
        } else {
            memory_score = (size_t)floor(100.0 * (double)(memory_limit_ - memory) / (double)(memory_limit_ - full_score_memory_));
        }

        size_t current_score = (size_t)floor((double)time_score * (double)memory_score / 100.0);
        if (current_score > score_) score_ = current_score;
    }
};

#endif
