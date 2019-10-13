// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <functional>
#include <memory>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "test_runner.h"

using namespace std;

struct Email {
    string from;
    string to;
    string body;

    friend ostream &operator<<(ostream &os, const Email &email) {
        os << email.from << "\n" << email.to
           << "\n" << email.body << "\n";
        return os;
    }
};

class Worker {
public:
    virtual ~Worker() = default;

    virtual void Process(unique_ptr<Email> email) = 0;

    virtual void Run() {
        // только первому worker-у в пайплайне нужно это имплементировать
        throw logic_error("Unimplemented");
    }

protected:
    // реализации должны вызывать PassOn, чтобы передать объект дальше
    // по цепочке обработчиков
    void PassOn(unique_ptr<Email> email) const {
        if (next_worker)
            next_worker->Process(move(email));
    }

public:
    void SetNext(unique_ptr<Worker> next) { next_worker = move(next); }

    unique_ptr<Worker> next_worker;
};

class Reader : public Worker {
    istream &in_;

public:
    explicit Reader(istream &in) : in_(in) {};

    void Run() override {
        while (in_) {
            unique_ptr<Email> email = make_unique<Email>();
            if(!getline(in_, email->from)) continue;
            if(!getline(in_, email->to)) continue;
            if(!getline(in_, email->body)) continue;
            Process(move(email));
        }
    }

    void Process(unique_ptr<Email> email) override { PassOn(move(email)); }

    // реализуйте класс
};

class Filter : public Worker {
public:
    using Function = function<bool(const Email &)>;

public:
    explicit Filter(Function func) : func__(move(func)) {};

    void Process(unique_ptr<Email> email) override {
        if (email && func__(*email)) PassOn(move(email));
    };

private:
    Function func__;
    // реализуйте класс
};

class Copier : public Worker {
    string recipient__;

public:
    explicit Copier(string recipient) : recipient__(std::move(recipient)) {}

    void Process(unique_ptr<Email> email) override {
        if (email) {
            if (recipient__ == email->to)
                PassOn(move(email));
            else {
                unique_ptr<Email> cpy_mail;
                cpy_mail = make_unique<Email>(*email);
                cpy_mail->to = recipient__;
                PassOn(move(email));
                PassOn(move(cpy_mail));
            }
        }
    };
    // реализуйте класс
};

class Sender : public Worker {
public:
    ostream &out_;

public:
    explicit Sender(ostream &out) : out_(out) {};

    void Process(unique_ptr<Email> email) override {
        if (email) {
            out_ << email->from << "\n";
            out_ << email->to << "\n";
            out_ << email->body << "\n";
        }
        PassOn(move(email));
    }
};

// реализуйте класс
class PipelineBuilder {
    unique_ptr<Worker> w_;
    Worker *last_w;

public:
    // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream &in) : last_w(nullptr), w_(nullptr) {
        w_ = unique_ptr<Worker>(new Reader(in));
        last_w = w_.get();
    }

    // добавляет новый обработчик Filter
    PipelineBuilder &FilterBy(Filter::Function filter) {
        last_w->SetNext(unique_ptr<Worker>(new Filter(std::move(filter))));
        last_w = last_w->next_worker.get();
        return *this;
    }

    // добавляет новый обработчик Copier
    PipelineBuilder &CopyTo(string recipient) {
        last_w->SetNext(unique_ptr<Worker>(new Copier(std::move(recipient))));
        last_w = last_w->next_worker.get();
        return *this;
    }

    // добавляет новый обработчик Sender
    PipelineBuilder &Send(ostream &out) {
        last_w->SetNext(unique_ptr<Worker>(new Sender(out)));
        last_w = last_w->next_worker.get();
        return *this;
    }

    // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build() { return move(w_); }
};

void TestSanity() {
    string input = (
            "erich@example.com\n"
            "richard@example.com\n"
            "Hello there\n"

            "erich@example.com\n"
            "ralph@example.com\n"
            "Are you sure you pressed the right button?\n"

            "ralph@example.com\n"
            "erich@example.com\n"
            "I do not make mistakes of that kind\n"
    );
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy(
            [](const Email &email) { return email.from == "erich@example.com"; });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput =
            ("erich@example.com\n"
             "richard@example.com\n"
             "Hello there\n"

             "erich@example.com\n"
             "ralph@example.com\n"
             "Are you sure you pressed the right button?\n"

             "erich@example.com\n"
             "richard@example.com\n"
             "Are you sure you pressed the right button?\n");
    ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}
