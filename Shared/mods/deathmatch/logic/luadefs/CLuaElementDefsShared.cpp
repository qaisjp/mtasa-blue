#include "StdInc.h"

int CLuaElementDefs::GetElementData(lua_State* luaVM)
{
    // any someVal, bool ok = getElementData ( element theElement, string key [, inherit = true] )

#ifdef MTA_CLIENT
    CClientEntity* pElement;
#else
    CElement* pElement;
#endif
    SString strKey;
    bool    bInherit;

    CScriptArgReader argStream(luaVM);
    argStream.ReadUserData(pElement);
    argStream.ReadString(strKey);
    argStream.ReadBool(bInherit, true);

    if (!argStream.HasErrors())
    {
        CLuaMain* pLuaMain = m_pLuaManager->GetVirtualMachine(luaVM);
        if (pLuaMain)
        {
            if (strKey.length() > MAX_CUSTOMDATA_NAME_LENGTH)
            {
                // Warn and truncate if key is too long
                m_pScriptDebugging->LogCustom(luaVM, SString("Truncated argument @ '%s' [%s]", lua_tostring(luaVM, lua_upvalueindex(1)),
                                                             *SString("string length reduced to %d characters at argument 2", MAX_CUSTOMDATA_NAME_LENGTH)));
                strKey = strKey.Left(MAX_CUSTOMDATA_NAME_LENGTH);
            }

#ifdef MTA_CLIENT
            CLuaArgument* pVariable = pElement->GetCustomData(strKey, bInherit);
#else
            CLuaArgument* pVariable = CStaticFunctionDefinitions::GetElementData(pElement, strKey, bInherit);
#endif
            if (pVariable)
            {
                pVariable->Push(luaVM);            // there is an associated warning here. read this function!
                lua_pushboolean(luaVM, true);
                return 2;
            }
        }
    }
    else
        m_pScriptDebugging->LogCustom(luaVM, argStream.GetFullErrorMessage());

    // Failed
    lua_pushboolean(luaVM, false);
    lua_pushboolean(luaVM, false);
    return 2;
}
