// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2021 The Bitcoin Core developers
// Copyright (c) 2021 Arian Romal
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "NY Times 22/Dec/2021 Facing Staff Shortages in Covid Surge";
    const CScript genesisOutputScript = CScript() << ParseHex("0401af210fa689ad5023690c80f3a49c8f13f8d45b8c857fb6798bc4a8e4d3eb4593f4d4604fa08dce601aaf0f470216fe1a76450b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 1051200;
		
        consensus.BIP34Height = 15330;
        consensus.BIP34Hash = uint256S("0xd3878b86586e3946acb57bda0cfc496f900d09a20d8e528085685241de6aad02");
        consensus.BIP65Height = 15330;
        consensus.BIP66Height = 15330;
		
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan = 10 * 60;
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 9;
        consensus.nMinerConfirmationWindow = 12;
		
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1642808504;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1643693000;

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1642808504;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1643693000;

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1642808504;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1643693000;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000da7202900e28");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x58a507685cfc3e045d95ed0ef21494430899ab3be0ee89655ae54c6be96813cf");
		

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xcb;
        pchMessageStart[1] = 0xcd;
        pchMessageStart[2] = 0x16;
        pchMessageStart[3] = 0xdf;
        nDefaultPort = 44814;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1640189773, 2000393295, 0x1e0ffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
	assert(genesis.hashMerkleRoot == uint256S("0xb0cc52d0ab21212a51f398ecc98384aa0ad56399d9bdcd5f6e13d87367fd1836"));
        assert(consensus.hashGenesisBlock == uint256S("0x2ac5a6ce63bcdbc8dcbe6b9d24f5db4691268257860c64556541454165cd1c50"));

        // Note that of those with the service bits flag, most only support a subset of possible options

    //    vSeeds.push_back(CDNSSeedData("", "", true));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,25);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,50);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,176);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (  0, uint256S("0x2ac5a6ce63bcdbc8dcbe6b9d24f5db4691268257860c64556541454165cd1c50"))
            (  1, uint256S("0x054bc280434994d9c24929b9cbb5dfccd12af77a7a492492e4743434a7e15dc7"))
            (  2, uint256S("0xa3c96c5d851181452d329e990171893a128272abd33c4d8a878013932736b480"))
            (  3, uint256S("0x15fc302b6ee856cc20f6bf9ed7e6b19732a81debd2f6cba741d17e2ebaad3773"))
            (  11, uint256S("0x9bd8b96e4f3cc3d74de40a1ed0827af1a337610f5b4e94c045d8864daa66d6b6"))
            (  100, uint256S("0xeddcb360c9fa1a539dc7a022738326996b54244bfee8499f560a0aa8410d0ba8"))
            (  1000, uint256S("0x4052907038d43e42032d6dfdbe9f76dcc859cf329e5fdcba7c2bfb13cef6f2ca"))
            (  5000, uint256S("0xbf5a564d3ddbca32a1623ac4262e35d179a9c31f8cbf0520ef045541baeae739"))
            (  10000, uint256S("0xed6078076cdc0678d37ecf965bc45185f2d58d843204171df5e488a691a8e1b3"))
            (  15000, uint256S("0x9e84be4a86219fc431081f5c6c83d1e70dec7f088c9601d00b465b8464654b97"))
            (  15322, uint256S("0xac5e309c62ffa04848e8aca6f17a7bee224f6067b2f8a8297eb518f9c04e29bd"))  
            (  15336, uint256S("0xf9e6c610a713d1569be7b6a373cc58ac1ce508a1b34f89a39a9ec9b6ee932857"))  // Segwit + CSV activation started       
            (  15348, uint256S("0x69e0a5676ead0b34001f8c7ab19d941c209d810e9a4a9dc63b019823d6d20fc1"))  // Segwit + CSV activation locked_in       
            (  15360, uint256S("0x828da61dc2b89146413dda15ea0878c88e460c235fa803be039409f4d4606187"))  // Segwit + CSV active 
            (  15371, uint256S("0x58a507685cfc3e045d95ed0ef21494430899ab3be0ee89655ae54c6be96813cf"))  // Segwit first block matured       
        };

        chainTxData = ChainTxData{
            // Data as of block b44bc5ae41d1be67227ba9ad875d7268aa86c965b1d64b47c35be6e8d5c352f4 (height 1155626).
            1643694410, // * UNIX timestamp of last known number of transactions
            15584,  // * total number of transactions between genesis and that timestamp
                    //   (the tx=... number in the SetBestChain debug.log lines)
            0.998782  // * estimated number of transactions per second after that timestamp
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 1051200;
		/*
        consensus.BIP34Height = 76;
        consensus.BIP34Hash = uint256S("8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573");
        consensus.BIP65Height = 76; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.BIP66Height = 76; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
		*/
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 10 * 60;
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
		/*
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517356801; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1517356801; // January 31st, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x000000000000000000000000000000000000000000000000000000054cb9e7a0");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x43a16a626ef2ffdbe928f2bc26dcd5475c6a1a04f9542dfc6a0a88e5fcf9bd4c"); //8711
		*/

        pchMessageStart[0] = 0xff;
        pchMessageStart[1] = 0xf2;
        pchMessageStart[2] = 0xcc;
        pchMessageStart[3] = 0xfa;
        nDefaultPort = 44815;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1640189773, 1003281, 0x1e0ffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
		assert(genesis.hashMerkleRoot == uint256S("0xb0cc52d0ab21212a51f398ecc98384aa0ad56399d9bdcd5f6e13d87367fd1836"));
        assert(consensus.hashGenesisBlock == uint256S("0xfa9c31fe0c4b414fbf029a8fd9895c297bca6d90214d9183228b5fde71b1a694"));

        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.push_back(CDNSSeedData("185.163.118.233", "185.163.118.233", true));
        vSeeds.push_back(CDNSSeedData("188.68.52.16", "188.68.52.16", true));
        vSeeds.push_back(CDNSSeedData("134.209.248.190", "134.209.248.190", true));
        //vSeeds.push_back(CDNSSeedData("188.68.52.16", "188.68.52.16", true));
        //vSeeds.push_back(CDNSSeedData("188.68.52.16", "188.68.52.16", true));
        //vSeeds.push_back(CDNSSeedData("188.68.52.16", "188.68.52.16", true));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0xfa9c31fe0c4b414fbf029a8fd9895c297bca6d90214d9183228b5fde71b1a694")),
        };

        chainTxData = ChainTxData{
            // Data as of block f2dc531da6be01f53774f970aaaca200c7a8317ee9fd398ee733b40f14e265d1 (height 8702)
            1640189773,
            0,
            0.0
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
		/*
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
		*/
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 10 * 60;
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
		/*
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;
		*/

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xaa;
        pchMessageStart[1] = 0xbb;
        pchMessageStart[2] = 0xbc;
        pchMessageStart[3] = 0xdd;
        nDefaultPort = 44816;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1640189773, 2, 0x207fffff, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
		assert(genesis.hashMerkleRoot == uint256S("0xb0cc52d0ab21212a51f398ecc98384aa0ad56399d9bdcd5f6e13d87367fd1836"));
        assert(consensus.hashGenesisBlock == uint256S("0xed8500f35253fdc445a5a271dfa9a86a308746d2aad7845d8a882d09d6017533"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0xed8500f35253fdc445a5a271dfa9a86a308746d2aad7845d8a882d09d6017533"))
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
