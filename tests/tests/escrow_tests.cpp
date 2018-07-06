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

#include <boost/test/unit_test.hpp>

#include <graphene/chain/protocol/escrow_contract.hpp>

#include "../common/database_fixture.hpp"

BOOST_FIXTURE_TEST_SUITE( escrow_tests, graphene::chain::database_fixture )

BOOST_AUTO_TEST_CASE( escrow_contract_expires )
{
   ACTORS((alice)(bob));

   int64_t init_balance(100000);

   transfer( committee_account, alice_id, graphene::chain::asset(init_balance) );

   uint16_t key_size = 256;
   std::vector<unsigned char> pre_image = generate_random_preimage(key_size);
   std::vector<unsigned char> key_hash = sha256(pre_image);

   graphene::chain::processed_transaction alice_trx;
   // Alice puts a contract on the blockchain
   {
      graphene::chain::escrow_htlc_create_operation create_operation;

      create_operation.amount = graphene::chain::asset( 10000 );
      create_operation.destination = bob_id;
      create_operation.epoch = fc::time_point::now() + fc::seconds(3);
      create_operation.hash = graphene::chain::escrow_htlc_create_operation::SHA256;
      create_operation.key_hash = key_hash;
      create_operation.key_size = key_size;
      create_operation.source = alice_id;
      trx.operations.push_back(create_operation);
      sign(trx, alice_private_key);
      PUSH_TX(db, trx, ~0);
      graphene::chain::signed_block blk = generate_block();
      // can we assume that alice's transaction will be the only one in this block?
      alice_trx = blk.transactions[0];
      trx.clear();
   }

   // verify funds on hold (make sure this can cover fees)
   BOOST_TEST_CHECK( check_balance(alice_id, graphene::chain::asset_id_type()) == 99000 );
   // make sure Alice can't get it back before the timeout
   {
      graphene::chain::escrow_htlc_update_operation update_operation;
      update_operation.update_issuer = alice_id;
      update_operation.trans_id = alice_trx.id();
      trx.operations.push_back(update_operation);
      sign(trx, alice_private_key);
      // one of these two lines should throw an exception
      PUSH_TX(db, trx, ~0);
      generate_block();
      trx.clear();
   }

   // make sure Bob (or anyone) can see the details of the transaction
   // let it expire (wait 3 seconds)
   // send an update operation to reclaim the funds (NOTE: key size will be checked)
   // verify funds return (what about fees?)
   // verify Bob cannot execute the contract after the fact
}

BOOST_AUTO_TEST_CASE( escrow_contract_fulfilled )
{
   // Alice puts a contract on the blockchain
   // verify funds on hold
   // Bob presents the hash
   // verify funds enter Bob's account
   // verify Alice cannot get her money back
}

BOOST_AUTO_TEST_SUITE_END()
