#pragma once

#include <functional>

namespace utility {
  namespace scope {
    namespace handle {
      class on_exit {
        public:
          on_exit(std::function<void()>&& exitFn) : exitFn(std::move(exitFn)) {}
          ~on_exit() { exitFn(); }



        private:
          std::function<void()> exitFn;
      };
    }


    handle::on_exit on_exit(std::function<void()>&& exitFn) { return handle::on_exit(std::move(exitFn)); }
  }
}