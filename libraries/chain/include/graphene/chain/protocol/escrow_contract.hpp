/*
 * Copyright (c) 2018 BitShares Foundation, and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once
#include <graphene/chain/protocol/base.hpp>
#include <graphene/chain/protocol/assert.hpp>

namespace graphene { namespace chain {

struct escrow_htlc_create_operation : base_operation
{
   enum hashtype { SHA256, RIPEMD160 };
   asset fee; // paid to network
   account_id_type source; // where the held monies are to come from
   account_id_type destination; // where the held monies will go if the pre-image is provided
   asset amount; // amount to hold
   hashtype hash; // hash algo used to create key_hash
   uint64_t key_hash; // hash of pre-image
   uint16_t key_size; // size of pre-image
   uint64_t epoch; // when the hold should be removed and monies placed back in source
   extensions_type extensions; // for future expansion
};

struct escrow_htlc_update_operation : base_operation
{
   transaction_id_type trans_id; // the transaction we're attempting to update
   account_id_type update_issuer; // who is attempting to update the transaction
   uint64_t preimage; // If the destination account is updating, he will need the preimage
   extensions_type extensions; // for future expansion
};

} }
