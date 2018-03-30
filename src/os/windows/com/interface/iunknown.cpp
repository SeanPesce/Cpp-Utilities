/*
    CONTRIBUTORS:
        Sean Pesce

    Wrapper class for Microsoft's IUnknown interface.

    References:
        https://msdn.microsoft.com/en-us/library/windows/desktop/ms680509(v=vs.85).aspx
*/
#ifdef _WIN32
#include "sp/os/windows/com/interface/iunknown.h"

__SP_NAMESPACE
namespace os  {
namespace win {
namespace com {

template <class interface_t>
const GUID iunknown<interface_t>::GUID = __uuidof(interface_t);

} // namespace com
} // namespace win
} // namespace os
__SP_NAMESPACE_CLOSE // namespace sp


#endif // _WIN32
