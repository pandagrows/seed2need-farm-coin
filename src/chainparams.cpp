// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2019 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "consensus/merkle.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>
#include <limits>


struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
//static Checkpoints::MapCheckpoints mapCheckpoints =
//    boost::assign::map_list_of
 //   (0, uint256("00000358424d602ae8564e2346a6305535b47a3e098a09bac96be7323f627b5b"))
//    ;
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
	(  0, uint256("0x00000b76fdf75f599ce14005fc484423949ae7f515a8dfe358ddfed6a1b52a6a"))
	;
	
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1582096211, // * UNIX timestamp of last checkpoint block
    0,          // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    100        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1740710,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1454124731,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

bool CChainParams::HasStakeMinAgeOrDepth(const int contextHeight, const uint32_t contextTime,
        const int utxoFromBlockHeight, const uint32_t utxoFromBlockTime) const
{
    // before stake modifier V2, the age required was 60 * 60 (1 hour). Not required for regtest
    if (!IsStakeModifierV2(contextHeight))
        return NetworkID() == CBaseChainParams::REGTEST || (utxoFromBlockTime + nStakeMinAge <= contextTime);

    // after stake modifier V2, we require the utxo to be nStakeMinDepth deep in the chain
    return (contextHeight - utxoFromBlockHeight >= nStakeMinDepth);
}

int CChainParams::FutureBlockTimeDrift(const int nHeight) const
{
    if (IsTimeProtocolV2(nHeight))
        // PoS (TimeV2): 14 seconds
        return TimeSlotLength() - 1;

    // PoS (TimeV1): 3 minutes
    // PoW: 2 hours
    return (nHeight > LAST_POW_BLOCK()) ? nFutureTimeDriftPoS : nFutureTimeDriftPoW;
}

bool CChainParams::IsValidBlockTimeStamp(const int64_t nTime, const int nHeight) const
{
    // Before time protocol V2, blocks can have arbitrary timestamps
    if (!IsTimeProtocolV2(nHeight))
        return true;

    // Time protocol v2 requires time in slots
    return (nTime % TimeSlotLength()) == 0;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xf0;
        pchMessageStart[1] = 0xc8;
        pchMessageStart[2] = 0x85;
        pchMessageStart[3] = 0xc6;
		
        vAlertPubKey = ParseHex("0453aded6c08f43bf71c9b41567bbbb6733557fc59698eebb58eda1963cad5e30c35b1955f5109de47f86df99b797316b8b286cef63afcad155f27a23e2e6c51eb");
        nDefaultPort = 2020;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // Seed2Need starting difficulty is 1 / 2^12
        bnProofOfStakeLimit = ~uint256(0) >> 24;
        bnProofOfStakeLimit_V2 = ~uint256(0) >> 20; // 60/4 = 15 ==> use 2**4 higher limit
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750; // 75%
        nRejectBlockOutdatedMajority = 950; // 95%
        nToCheckBlockUpgradeMajority = 1000; // Approximate expected amount of blocks in 7 days (1440*7.5)
        nMinerThreads = 0;
        nTargetSpacing = 1 * 90;                        // 90 Seconds
        nTargetTimespan = 40 * 60;                      // 40 minutes
        nTimeSlotLength = 15;                           // 15 seconds
        nTargetTimespan_V2 = 2 * nTimeSlotLength * 60;  // 30 minutes
        nMaturity = 150;								// 150 Blocks
        nStakeMinAge = 12* 60 * 60 * 2;                 // 24 hour
        nStakeMinDepth = 300;
        nFutureTimeDriftPoW = 7200;
        nFutureTimeDriftPoS = 180;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 21500000000 * COIN;
        nMinColdStakingAmount = 1 * COIN;

        nMasternodeCollateral = 1000000;
        strDevFundAddress = "WUEGhJYhrfnNd9sXMai49b19TMjMLrEsn9";
        nStakeInputMinimal = 5000 * COIN;
        nEnforceStakeInputMinimal = 10000;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 300;
        nSeed2needBadBlockTime = 1471401614; // Skip nBit validation of Block 259201 per PR #915
        nSeed2needBadBlocknBits = 0x1c056dac; // Skip nBit validation of Block 259201 per PR #915
        nModifierUpdateBlock = 0;
        nZerocoinStartHeight = 999999999;
        nZerocoinStartTime = 1873242920; // disabled
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 999999999; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 999999999; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 999999999; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 999999999; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 999999999; //!> The block that zerocoin v2 becomes active - roughly Tuesday, May 8, 2018 4:00:00 AM GMT
        nBlockDoubleAccumulated = 999999999;
        nEnforceNewSporkKey = 1525158000; //!> Sporks signed after Monday, August 26, 2019 11:00:00 PM GMT must use the new spork key
        nRejectOldSporkKey = 1527811200; //!> Fully reject old spork key after Thursday, September 26, 2019 11:00:00 PM GMT
        nBlockStakeModifierlV2 = 500000;
        nBIP65ActivationHeight = 10000;
        // Activation height for TimeProtocolV2, Blocks V7 and newMessageSignatures
        nBlockTimeProtocolV2 = 200000;

        // Public coin spend enforcement
        nPublicZCSpends = 0;

        // New P2P messages signatures
        nBlockEnforceNewMessageSignatures = nBlockTimeProtocolV2;

        // Blocks v7
        nBlockLastAccumulatorCheckpoint = 200000;
        nBlockV7StartHeight = nBlockTimeProtocolV2;

        // Fake Serial Attack
        nFakeSerialBlockheightEnd = -1;
        nSupplyBeforeFakeSerial = 0;   // zerocoin supply at block nFakeSerialBlockheightEnd

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "A sustainable agriculture is one which depletes neither the people nor the land.";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 250 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("042bedb84263e7729b8b39dabcafecb2102ba3e4d4a0c964fb67cea198370d26e30339719540adeac97dcfa372a2691b88af95e7069192a4c0c5507daa530fe873") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
        genesis.nVersion = 5;
        genesis.nTime = 1582096211; // Set to current epoch time
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 1406739; // SET to 0 to regen

        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("0x00000b76fdf75f599ce14005fc484423949ae7f515a8dfe358ddfed6a1b52a6a"));
        assert(genesis.hashMerkleRoot == uint256("0xa0c60c660565c3547e4ab133aa24925f9ea8373ba939b27817abb07b3de8bc38"));

        //vSeeds.push_back(CDNSSeedData("seed1",   "seed1.seed2need.me"));     // Primary DNS Seeder
        //vSeeds.push_back(CDNSSeedData("seed2",   "seed2.seed2need.me")); 
        //vSeeds.push_back(CDNSSeedData("seed3",   "seed3.seed2need.me"));   
		//vFixedSeeds.clear();
		vSeeds.clear();
		
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 73);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 40);
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 28);     // starting with 'C'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 55);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));
		/*
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;
		*/
		
		fMiningRequiresPeers = false; // default true
        fAllowMinDifficultyBlocks = false; // default false
        fDefaultConsistencyChecks = false; // default false
        fRequireStandard = true; // default true
        fMineBlocksOnDemand = true; // default false
        fSkipProofOfWorkCheck = true; // default false
        fTestnetToBeDeprecatedFieldRPC = false; // default false
        fHeadersFirstSyncingActive = false; // default false
		
        nPoolMaxTransactions = 3;
        nBudgetCycleBlocks = 43200; //!< Amount of blocks in a months period of time (using 1 minutes per) = (60*24*30)
        strSporkPubKey = "041f036f07bff0f469e9a535c743314b4a17429a230d809fe2401c21b00d6459662020b8c35ded7aa8fa478c39c110109d9701ab0641fd33b34f30b3dff7b5fe22";
        strSporkPubKeyOld = "0499A7AF4806FC6DE640D23BC5936C29B77ADF2174B4F45492727F897AE63CF8D27B2F05040606E0D14B547916379FA10716E344E745F880EDC037307186AA25B7";
        strObfuscationPoolDummyAddress = "SSDx9L7MAmg8yUfqMPTiRPjKJVnMyYz6mG";
        nStartMasternodePayments = 1574676000; // Monday, November 25, 2019 5:00:00 AM GMT-05:00

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMaxZerocoinPublicSpendsPerTransaction = 637; // Assume about 220 bytes each input
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 5; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zfarm to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
        nProposalEstablishmentTime = 60 * 60 * 24; // Proposals must be at least a day old to make it into a budget
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }

};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xbc;
        pchMessageStart[1] = 0xf5;
        pchMessageStart[2] = 0x81;
        pchMessageStart[3] = 0x9f;
        vAlertPubKey = ParseHex("042bedb84263e7729b8b39dabcafecb2102ba3e4d4a0c964fb67cea198370d26e30339719540adeac97dcfa372a2691b88af95e7069192a4c0c5507daa530fe873");
        nDefaultPort = 3020;
        nEnforceBlockUpgradeMajority = 4320; // 75%
        nRejectBlockOutdatedMajority = 5472; // 95%
        nToCheckBlockUpgradeMajority = 5760; // 4 days
        nMinerThreads = 0;
        nLastPOWBlock = 200;
        nSeed2needBadBlockTime = 1489001494; // Skip nBit validation of Block 259201 per PR #915
        nSeed2needBadBlocknBits = 0x1e0a20bd; // Skip nBit validation of Block 201 per PR #915
        nMaturity = 15;
        nStakeMinDepth = 100;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 0; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 300;
        nZerocoinStartTime = 1501776000;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 999999999; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 999999999; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 999999999; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 999999999; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 300; //!> The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1521604800; //!> Sporks signed after Wednesday, March 21, 2018 4:00:00 AM GMT must use the new spork key
        nRejectOldSporkKey = 1522454400; //!> Reject old spork key after Saturday, March 31, 2018 12:00:00 AM GMT
        nBlockStakeModifierlV2 = 1214000;
        nBIP65ActivationHeight = 100000;
        // Activation height for TimeProtocolV2, Blocks V7 and newMessageSignatures
        nBlockTimeProtocolV2 = 80000;

        // Public coin spend enforcement
        nPublicZCSpends = 0;

        // New P2P messages signatures
        nBlockEnforceNewMessageSignatures = nBlockTimeProtocolV2;

        // Blocks v7
        nBlockLastAccumulatorCheckpoint = nPublicZCSpends - 10;
        nBlockV7StartHeight = nBlockTimeProtocolV2;

        // Fake Serial Attack
        nFakeSerialBlockheightEnd = -1;
        nSupplyBeforeFakeSerial = 0;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1582097399;
        genesis.nNonce = 928499;

        hashGenesisBlock = genesis.GetHash();
		/*
		if(genesis.GetHash() != uint256("0x"))
        {
              printf("MSearching for genesis block...\n");
              uint256 hashTarget;
              hashTarget.SetCompact(genesis.nBits);
              while(uint256(genesis.GetHash()) > uint256(hashTarget))
              {
                  ++genesis.nNonce;
                  if (genesis.nNonce == 0)
                  {
                      printf("Testnet NONCE WRAPPED, incrementing time");
                      std::cout << std::string("Testnet NONCE WRAPPED, incrementing time:\n");
                      ++genesis.nTime;
                  }
                  if (genesis.nNonce % 10000 == 0)
                  {
                      printf("Testnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                  }
              }
              printf("Testnet block.nTime = %u \n", genesis.nTime);
              printf("Testnet block.nNonce = %u \n", genesis.nNonce);
              printf("Testnet block.hashMerkleRoot: %s\n", genesis.hashMerkleRoot.ToString().c_str());
              printf("Testnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
        }
		*/
        assert(hashGenesisBlock == uint256("0x000005e0299841dd4905a676faf7e79536f2273607728b84619c451e9d432ecf"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet seed2need addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet seed2need script addresses start with '8' or '9'
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 73);     // starting with 'W'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet seed2need BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet seed2need BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet seed2need BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        nBudgetCycleBlocks = 144; //!< Ten cycles per day on testnet
        strSporkPubKey = "042f3ee3fd6b795c7176f61e967fd7923d4c43f09d68720dfc05239f64b4765471f9a0049e1508f663c13bb454706e73947fae169c0ea41ddb6a97c4fa5b38690e";
        strSporkPubKeyOld = "04A8B319388C0F8588D238B9941DC26B26D3F9465266B368A051C5C100F79306A557780101FE2192FE170D7E6DEFDCBEE4C8D533396389C0DAFFDBC842B002243C";
        strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
        nStartMasternodePayments = 1560727481 + 500 * 120; 
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet

        nProposalEstablishmentTime = 60 * 5; // Proposals must be at least 5 mns old to make it into a test budget
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa3;
        pchMessageStart[1] = 0xca;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;
        nDefaultPort = 2220;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        nLastPOWBlock = 250;
        nMaturity = 100;
        nStakeMinAge = 0;
        nStakeMinDepth = 0;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 0;       //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 43199500 * COIN;
        nZerocoinStartHeight = 300;
        nBlockZerocoinV2 = 300;
        nZerocoinStartTime = 1501776000;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 999999999; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 999999999; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 999999999; //Last valid accumulator checkpoint
        nBlockStakeModifierlV2 = 255;
        nBlockTimeProtocolV2 = 999999999;

        // Public coin spend enforcement
        nPublicZCSpends = 0;

        // Blocks v7
        nBlockV7StartHeight = nPublicZCSpends + 1;
        nBlockLastAccumulatorCheckpoint = nPublicZCSpends - 10;

        // New P2P messages signatures
        nBlockEnforceNewMessageSignatures = 1;

        // Fake Serial Attack
        nFakeSerialBlockheightEnd = -1;

        //! Modify the regtest genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1573717670;
        genesis.nNonce = 3552;

        hashGenesisBlock = genesis.GetHash();
		/*
        if(genesis.GetHash() != uint256("0x000007624e5c18446f4487c593020cbbd39030905299376de26883f75c0caef4"))
        {
              printf("MSearching for genesis block...\n");
              uint256 hashTarget;
              hashTarget.SetCompact(genesis.nBits);
              while(uint256(genesis.GetHash()) > uint256(hashTarget))
              {
                  ++genesis.nNonce;
                  if (genesis.nNonce == 0)
                  {
                      printf("Regnet NONCE WRAPPED, incrementing time");
                      std::cout << std::string("Regnet NONCE WRAPPED, incrementing time:\n");
                      ++genesis.nTime;
                  }
                  if (genesis.nNonce % 10000 == 0)
                  {
                      printf("Regnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                  }
              }
              printf("Regnet block.nTime = %u \n", genesis.nTime);
              printf("Regnet block.nNonce = %u \n", genesis.nNonce);
              printf("Regnet block.hashMerkleRoot: %s\n", genesis.hashMerkleRoot.ToString().c_str());
              printf("Regnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
        }
		*/
        assert(hashGenesisBlock == uint256("0x000000b9eac8202a791e7f14627c197fbc89c84747411d7d536217518a7606af"));
        //assert(hashGenesisBlock == uint256("0x4f023a2120d9127b21bbad01724fdb79b519f593f2a85b60d3d79160ec5f29df"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        /* Spork Key for RegTest:
        WIF private key: 932HEevBSujW2ud7RfB1YF91AFygbBRQj3de3LyaCRqNzKKgWXi
        private key hex: bd4960dcbd9e7f2223f24e7164ecb6f1fe96fc3a416f5d3a830ba5720c84b8ca
        Address: yCvUVd72w7xpimf981m114FSFbmAmne7j9
        */
        strSporkPubKey = "043969b1b0e6f327de37f297a015d37e2235eaaeeb3933deecd8162c075cee0207b13537618bde640879606001a8136091c62ec272dd0133424a178704e6e75bb7";
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;

static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
