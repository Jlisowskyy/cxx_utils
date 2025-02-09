#include <cstdlib>

#include <CxxUtils/data_types/multi_vector.hpp>

#include <array>
#include <chrono>
#include <iostream>
#include <random>

using namespace CxxUtils;
static void TestMultiVecPerf()
{
    static constexpr size_t kNumObjects = 1e+6;
    static constexpr size_t kLoopRuns   = 1e+3;

    class BaseClass
    {
        public:
        virtual ~BaseClass()                 = default;
        virtual void Calculate(double delta) = 0;
        virtual double Get()                 = 0;
    };

    class DerivedClass1 final : public BaseClass
    {
        public:
        void Calculate(double delta) override { value_ += delta; }

        double Get() override { return value_; }

        private:
        double value_ = 0.0;
    };

    class DerivedClass2 final : public BaseClass
    {
        public:
        void Calculate(double delta) override { value_ -= delta; }

        double Get() override { return value_; }

        private:
        double value_ = 0.0;
    };

    class DerivedClass3 final : public BaseClass
    {
        public:
        void Calculate(double delta) override { value_ *= delta; }

        double Get() override { return value_; }

        private:
        double value_ = 1.0;
    };

    class DerivedClass4 final : public BaseClass
    {
        public:
        void Calculate(double delta) override { value_ /= delta; }

        double Get() override { return value_; }

        private:
        double value_ = 1.0;
    };

    class DerivedClassNoVirt1 final
    {
        public:
        FAST_CALL_ void Calculate(double delta) { value_ += delta; }
        FAST_CALL_ double Get() { return value_; }

        private:
        double value_ = 0.0;
    };

    class DerivedClassNoVirt2 final
    {
        public:
        FAST_CALL_ void Calculate(double delta) { value_ -= delta; }
        FAST_CALL_ double Get() { return value_; }

        private:
        double value_ = 0.0;
    };

    class DerivedClassNoVirt3 final
    {
        public:
        FAST_CALL_ void Calculate(double delta) { value_ *= delta; }
        FAST_CALL_ double Get() { return value_; }

        private:
        double value_ = 1.0;
    };

    class DerivedClassNoVirt4 final
    {
        public:
        FAST_CALL_ void Calculate(double delta) { value_ /= delta; }
        FAST_CALL_ double Get() { return value_; }

        private:
        double value_ = 1.0;
    };

    using factory_t = BaseClass* (*)();
    const std::array<factory_t, 4> kFactories{
        []() constexpr -> BaseClass* {
            return new DerivedClass1();
        },
        []() constexpr -> BaseClass* {
            return new DerivedClass2();
        },
        []() constexpr -> BaseClass* {
            return new DerivedClass3();
        },
        []() constexpr -> BaseClass* {
            return new DerivedClass4();
        }
    };

    std::mt19937_64 gen(std::random_device{}());

    std::vector<BaseClass*> objects;
    objects.reserve(kNumObjects);

    MultiVector<DerivedClassNoVirt1, DerivedClassNoVirt2, DerivedClassNoVirt3, DerivedClassNoVirt4> multi_vec;

    /* create objects */
    for (size_t i = 0; i < kNumObjects; ++i) {
        const size_t idx = std::uniform_int_distribution<size_t>(0, 3)(gen);
        objects.push_back(kFactories[idx]());

        switch (idx) {
            case 0:
                multi_vec.push_back(DerivedClassNoVirt1());
                break;

            case 1:
                multi_vec.push_back(DerivedClassNoVirt2());
                break;

            case 2:
                multi_vec.push_back(DerivedClassNoVirt3());
                break;

            case 3:
                multi_vec.push_back(DerivedClassNoVirt4());
                break;
        }
    }

    /* run test loop on dynamic objects */

    double total_sum{};
    auto t1 = std::chrono::high_resolution_clock::now();

    auto last_frame = std::chrono::steady_clock::now();
    for (size_t i = 0; i < kLoopRuns; ++i) {
        const auto new_frame = std::chrono::steady_clock::now();
        const auto delta_time_ms =
            std::chrono::duration_cast<std::chrono::microseconds>(new_frame - last_frame).count();
        last_frame = new_frame;

        for (size_t j = 0; j < kNumObjects; ++j) {
            objects[j]->Calculate(static_cast<double>(delta_time_ms) / 1e+6);
        }

        double sum = 0.0;
        for (size_t j = 0; j < kNumObjects; ++j) {
            sum += objects[j]->Get();
        }

        total_sum += sum;
    }

    auto t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Time spent for dynamic objects: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << std::endl;
    std::cout << "Control sum: " << total_sum << std::endl;

    /* run test loop on multi vector */
    total_sum = 0.0;

    t1 = std::chrono::high_resolution_clock::now();

    last_frame = std::chrono::steady_clock::now();
    for (size_t i = 0; i < kLoopRuns; ++i) {
        const auto new_frame = std::chrono::steady_clock::now();
        const auto delta_time_ms =
            std::chrono::duration_cast<std::chrono::microseconds>(new_frame - last_frame).count();
        last_frame = new_frame;

        multi_vec.Foreach([&delta_time_ms](auto& elem) FORCE_INLINE__ {
            elem.Calculate(static_cast<double>(delta_time_ms) / 1e+6);
        });

        double sum = 0.0;
        multi_vec.Foreach([&sum](auto& elem) FORCE_INLINE__ {
            sum += elem.Get();
        });
        total_sum += sum;
    }

    t2 = std::chrono::high_resolution_clock::now();

    std::cout << "Time spent for multi vector: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms" << std::endl;
    std::cout << "Control sum: " << total_sum << std::endl;
}

int main()
{
    TestMultiVecPerf();
    return EXIT_SUCCESS;
}
