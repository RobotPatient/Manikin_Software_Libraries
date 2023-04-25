#ifndef HAL_EXCEPTION_CONFIG_HPP
#define HAL_EXCEPTION_CONFIG_HPP

namespace hal::exception {
#define ASSEMBLE_THROW_MSG(line, tag, message, exception_type, action) \
  "[" tag  ": " str(line) "]: " "THROW! "  message "; exception: " str(exception_type) " Action: " str(action)
  
#define ASSEMBLE_ASSERT_MSG(line, tag, condition, action) \
  "[" tag  ": " str(line) "]: " "ASSERT! : " str(condition) " Action: " str(action)

#define str(s) #s
inline constexpr bool EnableSerialLogger = true;
inline constexpr bool EnableFlashLogger = false;
typedef enum {UNINITIALIZED, OUT_OF_BOUNDS, CONDITION_UNMET } ExceptionTypes;
typedef enum {WARN, HARD_RESET} ExceptionAction;

}  // namespace hal::exception
#endif