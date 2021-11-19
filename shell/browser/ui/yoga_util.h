#ifndef SHELL_BROWSER_UI_YOGA_UTIL_H_
#define SHELL_BROWSER_UI_YOGA_UTIL_H_

#include <string>

typedef struct YGNode *YGNodeRef;

namespace electron {

void SetYogaProperty(YGNodeRef node, const std::string& key, float value);
void SetYogaProperty(YGNodeRef node,
                     const std::string& key,
                     const std::string& value);

}  // namespace electron

#endif  // SHELL_BROWSER_UI_YOGA_UTIL_H_
