//------------------------------------------------------------------------------
/*
    This file is part of skywelld: https://github.com/skywell/skywelld
    Copyright (c) 2012-2014 Skywell Labs Inc.

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#include <BeastConfig.h>
#include <common/misc/NetworkOPs.h>
#include <common/base/StringUtilities.h>
#include <common/json/json_reader.h>
#include <common/core/LoadFeeTrack.h>
#include <protocol/TxFlags.h>
#include <protocol/Indexes.h>
#include <protocol/JsonFields.h>
#include <protocol/ErrorCodes.h>
#include <ledger/LedgerMaster.h>
#include <services/rpc/impl/KeypairForSignature.h>
#include <services/rpc/impl/TransactionSign.h>
#include <services/rpc/impl/Tuning.h>
#include <main/Application.h>
#include <services/rpc/impl/LegacyPathFind.h>
#include <services/net/RPCErr.h>
#include <transaction/paths/FindPaths.h>
#include <services/rpc/Context.h>
#include <network/peers/UniqueNodeList.h>
#include <services/rpc/impl/Handler.h>
//#include <protocol/JsonFields.h>
//#include <protocol/ErrorCodes.h>
//#include <services/net/RPCErr.h>
//#include <common/misc/NetworkOPs.h>
//#include <network/resource/Fees.h>
//#include <services/rpc/impl/LookupLedger.h>

namespace skywell {

// {
//   node: <domain>|<public_key>
// }
Json::Value doUnlDelete (RPC::Context& context)
{
    auto lock = std::unique_lock<std::recursive_mutex>(getApp().getMasterMutex());

    if (!context.params.isMember (jss::node))
        return rpcError (rpcINVALID_PARAMS);

    auto strNode = context.params[jss::node].asString ();
    SkywellAddress raNodePublic;

    if (raNodePublic.setNodePublic (strNode))
    {
        getApp().getUNL ().nodeRemovePublic (raNodePublic);
        return RPC::makeObjectValue ("removing node by public key");
    }
    else
    {
        getApp().getUNL ().nodeRemoveDomain (strNode);
        return RPC::makeObjectValue ("removing node by domain");
    }
}

} // skywell
