// rssyntax.h
// version 230304

#ifndef DEBUG
    #define DEBUG_MODE false
#else
    #define DEBUG_MODE true
#endif
#undef DEBUG

#include <cstdint>
#include <string>
#include <cstdio>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;
typedef std::uint8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;
typedef float f32;
typedef double f64;
using String = std::string;
#define Vec vector

#define loop while(true)
#define fn 
#define let const auto
#define var
#define mut 
#define let_mut auto
#define self (*this)
#define mod namespace
#define use using namespace
#define print printf
//#define   X..Y             (u32 i = X; i < Y; i++)
//#define   X as Y           static_cast<Y>(X)
//#define   if X {           if (X) {
//#define   while X {        while (X) {
//#define   for X..Y {       if (auto i = X__iterator_begin; X != X_iterator_end; i += 1) {
//#define   let mut X: Y     Y X



mod rss{
    /// This allows a program to terminate immediately and provide feedback to the caller of the program with a provided string.
    fn void panic(String errMsg, u32 errTimeMS = 5000) {
        errTimeMS = (errTimeMS >= 333 || errTimeMS == 0 ? errTimeMS : 5000) / 333;
        errTimeMS /= 2;
        std::cerr << "\n" << errMsg << std::flush;
        if (errTimeMS != 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(333));
            for (u32 i = 0; i < errTimeMS; i++) {
                std::cerr << "\r" << errMsg << "     " << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(333));
                std::cerr << "\r" << errMsg << " <---" << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(333));
            }
            std::cerr << "\r" << errMsg << "     \n" << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(333));
        } else {
            std::cerr << "\n" << std::flush;
        }
        if (!DEBUG_MODE) {
            std::exit(EXIT_FAILURE);
        }
    }

    /// Internal 'overload', do not know how to make private.
    template<typename V>
    fn void panic_wv(String errMsg, V valMsg, u32 errTimeMS = 5000) {
        String parsedValMsg;
        if (false 
        || typeid(valMsg) == typeid(bool) 
        || typeid(valMsg) == typeid(u8) 
        || typeid(valMsg) == typeid(u16) 
        || typeid(valMsg) == typeid(u32) 
        || typeid(valMsg) == typeid(u64) 
        || typeid(valMsg) == typeid(i8) 
        || typeid(valMsg) == typeid(i16) 
        || typeid(valMsg) == typeid(i32) 
        || typeid(valMsg) == typeid(i64)
        || typeid(valMsg) == typeid(f32)
        || typeid(valMsg) == typeid(f64)
        ) {parsedValMsg = std::to_string(valMsg);}
        //else if (typeid(valMsg) == typeid(String) ) { parsedValMsg = static_cast<String>(valMsg);}
        //else if (typeid(valMsg) == typeid(char *)) { parsedValMsg = static_cast<char>(valMsg);}
        else { panic("Invalid value message for private panic overload"); }
        errTimeMS = (errTimeMS >= 333 || errTimeMS == 0 ? errTimeMS : 5000) / 333;
        errTimeMS /= 2;
        std::cerr << "\n" << errMsg << ": " << parsedValMsg << std::flush;
        if (errTimeMS != 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(333));
            for (u32 i = 0; i < errTimeMS; i++) {
                std::cerr << "\r" << errMsg << ": " << parsedValMsg << "     " << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(333));
                std::cerr << "\r" << errMsg << ": " << parsedValMsg << " <---" << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(333));
            }
            std::cerr << "\r" << errMsg << ": " << parsedValMsg << "     \n" << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(333));
        } else {
            std::cerr << "\n" << std::flush;
        }
        if (!DEBUG_MODE) {
            std::exit(EXIT_FAILURE);
        }
    }

    /// Asserts that a boolean expression is true at runtime.
    /// - - -
    /// Assertions are always checked in both debug and release builds. See 'debug_assert()' for assertions that are not enabled in release builds.
    fn void assert(bool expr, String assertMsg = "Assert failed, use 'CRTL + F' and type 'assert(' to seek origin") {
        if (expr == true) {
            return;
        }
        panic(assertMsg);
    }

    /// Asserts that a boolean expression is true at runtime in release builds.
    fn void debug_assert(bool expr, String assertMsg = "Debug assert failed, use 'CRTL + F' and type 'assert(' to seek origin") {
        if (expr == true) {
            return;
        }
        panic(assertMsg, DEBUG_MODE);
    }

    /// Asserts that two expressions are equal to each other at runtime.
    /// - - -
    /// Eq assertions are always checked in both debug and release builds. See 'debug_assert_eq()' for assertions that are not enabled in release builds.
    template<typename A, typename B>
    fn void assert_eq(A expr_a, B expr_b, String assertMsg = "Assert eq failed, use 'CRTL + F' and type 'assert_eq(' to seek origin") {
        if (expr_a == expr_b) {
            return;
        }
        std::cerr << "\nleft: `" << expr_a << "`,\nright: `" << expr_b << "`," << std::flush;
        panic(assertMsg);
    }

    /// Asserts that two expressions are equal to each other at runtime in release builds.
    template<typename A, typename B>
    fn void debug_assert_eq(A expr_a, B expr_b, String assertMsg = "Debug assert eq failed, use 'CRTL + F' and type 'assert_eq(' to seek origin") {
        if (expr_a == expr_b) {
            return;
        }
        std::cerr << "\nleft: `" << expr_a << "`,\nright: `" << expr_b << "`," << std::flush;
        panic(assertMsg, DEBUG_MODE);
    }

    /// Indicates unfinished code by panicking and conveys an intent of implementing the functionality later.
    /// - - - 
    /// This can be useful if you are prototyping and are just looking to have your code typecheck.
    fn void todo() {
        panic("Part of the code is unfinished, use 'CRTL + F' and type 'todo(' to seek origin"); 
    }

    /// Indicates unimplemented code by panicking, does not convey any intent.
    /// - - -
    /// This can be useful if you are prototyping and are just looking to have your code typecheck.
    fn void unimplemented() {
        panic("This art of the code is unimplemented, use 'CRTL + F' and type 'unimplemented(' to seek origin");
    }

    enum ResultEnum {
        /// Contains the error value.
        Err,
        /// Contains the success value.
        Ok 
        };
    enum OptionEnum { 
        /// No value.
        None,
        /// Some value of type T.
        Some 
    };

    /// Type Option represents an optional value: every Option is either Some and contains a value, or None, and does not.
    template <typename T>
    struct Option {
        OptionEnum Enum;
        private:
            T Value;
        public:
            /// Constructs an Option<T> holding a None value
            Option() {
                self.Enum = OptionEnum::None;
            }

            /// Constructs an Option<T> given a value
            /// - - -
            /// If the value given is None it will construct an Option holding a None value, otherwise constructs an option of the giiven Some value.
            Option(T expr) {
                switch (expr) {
                case OptionEnum::None:
                    self.Enum = OptionEnum::None;
                    break;
                case OptionEnum::Some:
                    panic("Cannot construct 'Option<T>' from 'Some', as 'Option<Some>' and 'Option<None>' cannot exist.");
                    break;
                default:
                    self.Value = expr;
                    self.Enum = OptionEnum::Some;
                }
            }

            /// Returns the contained Some value. Panics if the value is a None with a custom panic message provided by msg.
            fn T expect(const String msg) const {
                if (self.Enum == OptionEnum::None) { rss::panic(msg); }
                return self.Value;
            }

            /// Returns the contained Some value. Panics if the self value equals None.
            /// - - -          
            /// Because this function may panic, its use is generally discouraged. Instead, prefer to use pattern matching and handle the None case explicitly, or call unwrap_or, or unwrap_or_else.
            fn T unwrap() const {
                if (self.Enum == OptionEnum::None) { rss::panic("Called unwrap on 'None' value, use 'CRTL + F' and type '.unwrap()' to seek origin"); }
                return self.Value;
            }

            /// Returns the contained Some value or a provided default.
            /// - - -
            /// Arguments passed to unwrap_or are eagerly evaluated; if you are passing the result of a function call, it is recommended to use unwrap_or_else, which is lazily evaluated.
            fn void unwrap_or(const T expr) const {
                if (self.Enum == OptionEnum::None) { return expr; }
                return self.Value;
            }

            /// Returns the contained Some value or a provided default.
            fn T unwrap_or_else(const T expr) const {
                panic("'unwrap_or()' is lazily evalueted! That is unsupported in c++, use 'unwrap_or()', use 'CRTL + F' and type '.unwrap_or_else()' to seek origin");
            }
            

            /// Inserts value into the option if it is None, then returns a mutable reference to the contained value.
            fn T get_or_insert(const T expr) {
                if (self.Enum == OptionEnum::None) { 
                    self.Value = expr; 
                    self.Enum = OptionEnum::Some;
                }
                return self.Value;
            }

            /// Takes the value out of the option, leaving a None in its place.
            fn Option<T> take() {
                /* let_mut */ Option<T> oldValue;
                if (self.Enum == OptionEnum::Some) {
                    oldValue = self.Value;
                }
                self.Enum = OptionEnum::None;
                return oldValue;
            }

            /// Replaces the actual value in the option by the value given in parameter, returning the old value if present.
            fn Option<T> replace(const T expr) {
                /* let_mut */ Option<T> oldValue;
                if (self.Enum == OptionEnum::Some) {
                    oldValue = self.Value;
                }
                self.Value = expr;
                self.Enum = OptionEnum::Some;
                return oldValue;
            }

            /// Converts from Option<Option<T>> to Option<T>.
            /// - - -
            /// Flattening only removes one level of nesting at a time.
            fn T flatten() const {
            //template<typename V> // move above funciton declaration if uncommented
                self.unwrap();
                if (false 
                //|| typeid(self.Value) == typeid(Option<T>)
                //|| typeid(self.Value) == typeid(Option<V>)
                || typeid(self.Value) == typeid(Option<bool>)
                || typeid(self.Value) == typeid(Option<i32>)
                || typeid(self.Value) == typeid(Option<u32>)
                || typeid(self.Value) == typeid(Option<f64>)
                || typeid(self.Value) == typeid(Option<char>) 
                || typeid(self.Value) == typeid(Option<String>)
                //|| typeid(self.Value) == typeid(Option<std::vector<V>>)
                ) /*(self.Value.Enum == Some)*/  
                {} else {
                    rss::panic("Called flatten on 'Option<T>' instead of 'Option<Option<T>>'");
                }
                return self.Value;
            }

            /// Returns true if the option is a None value.
            fn bool is_none() const {
                return self.Enum == OptionEnum::None;
            }

            /// Returns true if the option is a Some value.
            fn bool is_some() const {
                return self.Enum == OptionEnum::Some;
            }
    };

    /// Result<T, E> is the type used for returning and propagating errors. It has the variants, Ok(T), representing success and containing a value, and Err(E), representing error and containing an error value.
    template <typename T, typename E>
    struct Result {
        ResultEnum Enum;
        private:
            T OkValue;
            E ErrValue;
        
        public:
            /// Constructs a Result<T, E> given the error status and a value
            Result(ResultEnum newEnum, T expr) {
                    if (newEnum == ResultEnum::Ok)  { self.OkValue = expr; }
                    if (newEnum == ResultEnum::Err) { self.ErrValue = expr; }
                self.Enum = newEnum;
            }

            /// Returns the contained Ok value. Panics if the value is an Err, with a panic message including the passed message, and the content of the Err.
            /// - - -
            /// Because this function may panic, its use is generally discouraged. Instead, prefer to use pattern matching and handle the Err case explicitly, or call unwrap_or, or unwrap_or_else.
            T expect(String msg) const {
                if (self.Enum == ResultEnum::Err) { /* rss::panic_wv(msg, self.ErrValue) */ panic(msg); }
                return self.OkValue;
            }

            /// Returns the contained Err value. Panics if the value is an Err, with a panic message including the passed message, and the content of the Ok.
            /// - - -
            /// Because this function may panic, its use is generally discouraged. Instead, prefer to use pattern matching and handle the Ok case explicitly, or call unwrap_or, or unwrap_or_else.
            T expect_err(const String msg) const {
                if (self.Enum == ResultEnum::Ok) { /* rss::panic_wv(msg, self.OkValue); */  panic(msg); }
                return self.ErrValue;
            }

            /// Returns the contained Ok value. Panics if the value is an Err, with a panic message provided by the Err’s value..
            /// - - -          
            /// Because this function may panic, its use is generally discouraged. Instead, prefer to use pattern matching and handle the Err case explicitly, or call unwrap_or, or unwrap_or_else.
            T unwrap() const {
                String tmpStr = "Called '.unwrap()' on 'Err' value: ";
                if (self.Enum == ResultEnum::Err) { /* rss::panic_wv(tmpStr, self.ErrValue); */  panic(tmpStr); }
                return self.OkValue;
            }

            // Returns the contained Err value. Panics if the self value equals Ok.
            /// - - -          
            /// Because this function may panic, its use is generally discouraged. Instead, prefer to use pattern matching and handle the Err case explicitly, or call unwrap_or, or unwrap_or_else.
            T unwrap_err() const {
                String tmpStr = "Called '.unwrap_err()' on 'Ok' value: ";
                if (self.Enum == ResultEnum::Ok) {  /* rss::panic_wv(tmpStr, self.OkValue); */  panic(tmpStr); }
                return self.ErrValue;
            }


            /// Returns the contained Ok value or a provided default.
            /// - - -
            /// Arguments passed to unwrap_or are eagerly evaluated; if you are passing the result of a function call, it is recommended to use unwrap_or_else, which is lazily evaluated.
            void unwrap_or(const T expr) const {
                if (self.Enum == ResultEnum::Err) { return expr; }
                return self.OkValue;
            }

            /// Returns the contained Ok value or a provided default.
            T unwrap_or_else(const T expr) const {
                panic("'unwrap_or()' is lazily evalueted! That is unsupported in c++, use 'unwrap_or()', use 'CRTL + F' and type '.unwrap_or_else()' to seek origin");
            }
            
            /// Converts from Result<T, E> to Option<T> ignoring the error, if any.
            Option<T> ok() const {
                /* let_mut */ Option<T> futureOption;
                if (self.Enum == ResultEnum::Ok) {
                    futureOption = OkValue;    
                }
                return futureOption;
            }
            
            /// Converts from Result<T, E> to Option<E> ignoring the success value, if any.
            Option<E> err() const {
                /* let_mut */ Option<E> futureOption;
                if (self.Enum == ResultEnum::Err) {
                    futureOption = ErrValue;    
                }
                return futureOption;
            }

            
            /// Converts from Result<Result<T, E>, E> to Result<T, E>.
            /// - - -
            /// Flattening only removes one level of nesting at a time.
            T flatten() {
            //template<typename V>  // move above funciton declaration if uncommented
                /*self.unwrap();
                if (false 
                //|| typeid(self.Value) == typeid(Result<i32, T>)
                //|| typeid(self.Value) == typeid(Result<i32, V>)
                || typeid(self.Value) == typeid(Result<i32, bool>)
                || typeid(self.Value) == typeid(Result<i32, i32>)
                || typeid(self.Value) == typeid(Result<i32, u32>)
                || typeid(self.Value) == typeid(Result<i32, f64>)
                || typeid(self.Value) == typeid(Result<i32, char>) 
                || typeid(self.Value) == typeid(Result<i32, String>)
                //|| typeid(self.Value) == typeid(Result<i32, std::vector<V>>)
                )
                {} else {
                rss::panic("Called flatten on 'Result<T, E>' instead of 'Option<Result<T, E>, E>'");
                }
                return self.Value;*/
                unimplemented();
            }

            /// Returns true if the result is Err.
            bool is_err() const {
                return self.Enum == ResultEnum::Err;
            }

            /// Returns true if the result is Ok.
            bool is_ok() const {
                return self.Enum == ResultEnum::Ok;
            }
    };
}



