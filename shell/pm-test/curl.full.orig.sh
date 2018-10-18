#!/bin/sh
#host="172.23.5.138:8080"
host="192.168.33.2:80"
urlPrefix="http://${host}/mit"
token=""
function login()
{
token=$(curl -i -X POST \
  "http://${host}/auth?actn=lgin" \
  -H 'Cache-Control: no-cache' \
  -H 'Content-Type: application/json;ext=nn' \
  -H 'Postman-Token: 854f3956-4f30-4327-9b4b-6303deae2511' \
  -d '{"in":{"pswd":"CHGME.1a","un":"admin"}}' | grep X-Auth-Token)
echo $token
}

function doCurl()
{
local    url=$1
local    data=$2

time ( curl -i -X POST \
  "$url" \
  -H 'Cache-Control: no-cache' \
  -H 'Content-Type: application/json;ext=nn' \
  -H 'Postman-Token: f14af274-1ac5-454c-b59e-e57c6fcb44c4' \
  -H "$token" \
  -d "${data}" 2>/dev/null | grep HTTP >&2 )>&2
}

function createSubnetwork()
{
echo >&2      
echo "*       createSubnetwork" >&2        
local url="/f3sn"
local data="$(cat <<EOF
{
}
EOF
)"

doCurl "${urlPrefix}${url}" "${data}"
echo $url
}

function createNetworkElement()
{
echo >&2      
echo "*       createNetworkElement" >&2        
local url="/f3sn/f3ne/1"
local data="$(cat <<EOF
{
"neId":1,
"neType":"0",
"neDescription": "",
"neName": "FSP 150-XG480",
"neContact": "",
"neCmdPromptPrefix": "{\"fl\":[\"{path}\"],\"sl\":[\"{username}\",\"@\",\"{node-name}\"]}",
}
EOF
)"

doCurl "${urlPrefix}${url}" "${data}"
echo $url
}
function createShelf()
{
echo >&2      
echo "*       createShelf" >&2        
local url="/f3sn/f3ne/1/f3shelf/1"
local data="$(cat <<EOF
{
    "ledControl": "1",
    "shelfId": 1,
    "shelfType": "1",
    "alias": "11",
    "description": "11",
    "adminState": "1",
    "dnm": "1"
}
EOF
)"

doCurl "${urlPrefix}${url}" "${data}"
echo $url
}

function createSlot()
{
echo >&2      
echo "*       create slot $2" >&2        
local shelfUrl=$1;
local slotId=$2
local url="$shelfUrl/f3slot/$slotId"
local data="$(cat <<EOF
{
    "slotId":$slotId,
    "adminState":"1",
    "description":"SLOT",
    "dnm":"$slotId",
    "f3card": {
        "cardInstanceType": "1",
        "f3eoe": {
            "ieee8023azControl": false,
            "fineGrainPmIntervalType": "1",
            "highTemperatureThreshold": 1,
            "lowTemperatureThreshold": 1,
            "snmpDyingGaspControl": false,
            "dnm": "1"
        },
        "f3dcn": {
            "alias": "",
            "configSpeed": "1",
            "mdixType": "1",
            "mediaType": "1",
            "mtu": 1500,
            "adminState": "1",
            "dnm": "1"
        },
        "alias": "1",
        "description": "ETHERNET CARD",
        "adminState": "1",
        "dnm": ""
    }
}
EOF
)"

doCurl "${urlPrefix}${url}" "${data}"
echo $url
}

function createPort()
{
echo >&2
echo "*       create port $2" >&2        
local slotUrl=$1
local portId=$2
local url="${slotUrl}/f3card/f3eoe/f3l2port/${portId},1"
local data="$(cat <<EOF
{
    "autoDiagnostics": false,
    "f3efmoam": {
        "control": false,
        "localMode": "1",
        "dnm": "1"
    },
    "f3elmist": {
        "dnm": "1"
    },
    "f3l2elmi": {
        "asyncStatusEnabled": false,
        "elmiEnabled": false,
        "minAsyncMessageInterval": 0,
        "n393StatusCounter": 2,
        "t392PollVerificationTimer": 0,
        "dnm": "1"
    },
    "portId": ${portId},
    "portType": "1",
    "f3wdg": {
        "dnm": "1"
    },
    "afpMode": "1",
    "encapPcpDeEncMode": "1",
    "expDeEncMode": "1",
    "independentShaperBandwidthControl": false,
    "ipPreDeEncMode": "1",
    "lowLatencyQueuingControl": false,
    "mirrorBufferSize": 1,
    "mirrorMonitorPortEnabled": false,
    "mirrorSourceEnabled": false,
    "pcpDeEncMode": "1",
    "portMode": "1",
    "portVlan": "1-*",
    "prioMapProfile": "1",
    "qinqEtherType": 1,
    "rxDeiAction": "1",
    "rxDeiOuterTag": "1",
    "rxPauseControl": false,
    "shapedSpeed": 1,
    "shapingControl": false,
    "svcMode": "1",
    "swapPriorityVid": 1,
    "txDeiAction": "1",
    "txDeiOuterTag": "1",
    "txPauseControl": false,
    "wfqGroupCos": 1,
    "wfqGroupEir": 1,
    "wfqSegmentationCos": 1,
    "egressPopPortVidControl": false,
    "egressSwapPriorityVidControl": false,
    "egressVlanTrunkingControl": false,
    "ingressPushPortVidControl": false,
    "ingressSwapPriorityVidControl": false,
    "a2nPushPvidControl": false,
    "a2nSwapPriorityVidControl": false,
    "n2aPopPvidControl": false,
    "n2aSwapPriorityVidControl": false,
    "n2aVlanTrunkControl": false,
    "sfpWdmChannel": "1",
    "xgePhyInfo": "1",
    "configSpeed": "1",
    "mdixType": "1",
    "mediaType": "1",
    "mtu": 1,
    "f3cpd": {
        "l2ptTunnelMac": "00:11:22:33:44:55",
        "bpduDispType": "1",
        "bridgeGroupDispType": "1",
        "bridgeReservedDispType": "1",
        "cdpDispType": "1",
        "ciscoGroupDispType": "1",
        "customerLldpDispType": "1",
        "dtpDispType": "1",
        "efmOamDispType": "1",
        "elmiDispType": "1",
        "garpDisptype": "1",
        "garpGroupDispType": "1",
        "gmrpDispType": "1",
        "gvrpDispType": "1",
        "islDispType": "1",
        "l2ptDispType": "1",
        "lacpDispType": "1",
        "lacpMarkerDispType": "1",
        "lanbridgesDispType": "1",
        "mac00DispType": "1",
        "mac01DispType": "1",
        "mac02DispType": "1",
        "mac03DispType": "1",
        "mac04DispType": "1",
        "mac05DispType": "1",
        "mac06DispType": "1",
        "mac07DispType": "1",
        "mac08DispType": "1",
        "mac09DispType": "1",
        "mac0aDispType": "1",
        "mac0bDispType": "1",
        "mac0cDispType": "1",
        "mac0dDispType": "1",
        "mac0eDispType": "1",
        "mac0fDispType": "1",
        "nearestLldpDispType": "1",
        "nonTpmrLldpDispType": "1",
        "pagpDispType": "1",
        "pauseDispType": "1",
        "portAuthenDispType": "1",
        "ptpNonFwdTagged": "1",
        "ptpNonFwdUntagged": "1",
        "pvstpPlusDispType": "1",
        "ssmDispType": "1",
        "udldDispType": "1",
        "uplinkfastDispType": "1",
        "vlanBridgeDispType": "1",
        "vtpDispType": "1",
        "dnm": "1"
    },
    "f3llf": {
        "control": false,
        "delay": 1,
        "llfTxActionType": "1",
        "localLinkId": 1,
        "remoteLinkIds": "1",
        "signalType": "1",
        "triggerType": "1",
        "dnm": "1"
    },
    "alias": "1",
    "description": "1",
    "adminState": "1",
    "dnm": "3"
}
EOF
)"
doCurl "${urlPrefix}${url}" "${data}"
echo $url
}

function createFp()
{
echo >&2
echo "*       create fp $1 $2" >&2        
local portUrl=$1
local flowpointId=$2
local url="${portUrl}/f3fp/${flowpointId}"
local data="$(cat <<EOF
{
    "eflow": {
        "control": false,
        "identification": "1",
        "rateThreshold": 0,
        "dnm": "1"
    },
    "flowpointId": ${flowpointId},
    "fpcpd": {
        "bpduDispType": "1",
        "bridgeGroupDispType": "1",
        "bridgeReservedDispType": "1",
        "cdpDispType": "1",
        "ciscoGroupDispType": "1",
        "customerLldpDispType": "1",
        "dtpDispType": "1",
        "efmOamDispType": "1",
        "elmiDispType": "1",
        "garpDisptype": "1",
        "garpGroupDispType": "1",
        "gmrpDispType": "1",
        "gvrpDispType": "1",
        "islDispType": "1",
        "l2ptDispType": "1",
        "lacpDispType": "1",
        "lacpMarkerDispType": "1",
        "lanbridgesDispType": "1",
        "mac00DispType": "1",
        "mac01DispType": "1",
        "mac02DispType": "1",
        "mac03DispType": "1",
        "mac04DispType": "1",
        "mac05DispType": "1",
        "mac06DispType": "1",
        "mac07DispType": "1",
        "mac08DispType": "1",
        "mac09DispType": "1",
        "mac0aDispType": "1",
        "mac0bDispType": "1",
        "mac0cDispType": "1",
        "mac0dDispType": "1",
        "mac0eDispType": "1",
        "mac0fDispType": "1",
        "nearestLldpDispType": "1",
        "nonTpmrLldpDispType": "1",
        "pagpDispType": "1",
        "pauseDispType": "1",
        "portAuthenDispType": "1",
        "ptpNonFwdTagged": "1",
        "ptpNonFwdUntagged": "1",
        "pvstpPlusDispType": "1",
        "ssmDispType": "1",
        "udldDispType": "1",
        "uplinkfastDispType": "1",
        "vlanBridgeDispType": "1",
        "vtpDispType": "1",
        "dnm": "1"
    },
    "fpelmi": {
        "dnm": "1"
    },
    "autoBandwidthConfigEnabled": false,
    "autoCirPercentage": 100,
    "egressShapingType": "1",
    "guaranteedBandwidth": 1,
    "hierarchicalCosEnabled": false,
    "maximumBandwidth": 1,
    "egressInnerPriomapEnabled": false,
    "egressOuterPriomapEnabled": false,
    "ingressMultiCosEnabled": false,
    "priomapProfile": "1",
    "usePortPriomapProfile": true,
    "ctagControl": "1",
    "cvlanTag": "1-0",
    "defaultMemberEnabled": false,
    "ingressUntaggedFrameEnabled": false,
    "ingressVlanMemberList": "1",
    "stagControl": "1",
    "svlanTag": "1-0",
    "macLearningEnabled": false,
    "maxFpFwdTableEntries": 16384,
    "protectLearningControl": "1",
    "elmiEvcIdentifier": "1",
    "broadcastRateLimit": 64000,
    "broadcastRateLimitEnabled": false,
    "combinedRateLimit": 64000,
    "combinedRateLimitEnabled": false,
    "multicastRateLimit": 64000,
    "multicastRateLimitEnabled": false,
    "alias": "",
    "associatedElp": "1",
    "frameForwardEnabled": true,
    "loopAvoidance": "1",
    "sesFlrThreshold": 30,
    "splitHorizonGroup": "1",
    "adminState": "1",
    "dnm": "1"
}
EOF
)"
doCurl "${urlPrefix}${url}" "${data}"
echo $url
}

function createFpPolicer()
{
echo >&2
echo "*       create fp policer $1 $2" >&2        
local fpUrl=$1
local policerId=$2
local url="${fpUrl}/fpmeter/${policerId}"
local data="$(cat <<EOF
{
    "policerId": ${policerId},
    "envelopeInstance": "1",
    "maxCir": 1,
    "maxEir": 1,
    "rank": 1,
    "associatedShaper": "1",
    "cbs": 1,
    "cir": 1,
    "colorMarkingFlag": false,
    "colorMode": "1",
    "couplingFlag": false,
    "direction": "1",
    "ebs": 1,
    "eir": 1,
    "policingEnabled": false,
    "adminState": "1",
    "dnm": "1"
}
EOF
)"
doCurl "${urlPrefix}${url}" "${data}"
echo $url
}

function createFpShaper()
{
echo >&2
echo "*       create fp shaper $1 $2" >&2        
local fpUrl=$1
local shaperId=$2
local url="${fpUrl}/fpshaper/${shaperId}"
local data="$(cat <<EOF
{
    "shaperId": ${shaperId},
    "bufferSize": 1,
    "cir": 1,
    "cos": 0,
    "direction": "1",
    "eir": 1,
    "adminState": "1",
    "dnm": "1"
}
EOF
)"
doCurl "${urlPrefix}${url}" "${data}"
echo $url
}

function createFlow()
{
echo >&2
echo "*       create flow $1 $2" >&2        
local slotUrl=$1
local flowId=$2
local url="${slotUrl}/f3card/f3eoe/f3l2flow/${flowId}"
local data="$(cat <<EOF
{
    "flowId": ${flowId},
    "agingTimer": 1,
    "autoBandwidthConfigControl": false,
    "autoCirPercentage": 0,
    "circuitName": "1",
    "configType": "1",
    "cosValue": 0,
    "ctagControl": "1",
    "ctagMatchRxPrio": false,
    "ctagVlan": "1-*",
    "esFlThreshold": 1,
    "flowMode": "1",
    "flowPort1": "${slotUrl}/f3card/f3eoe/f3l2port/1,1",
    "flowPort2": "${slotUrl}/f3card/f3eoe/f3l2port/2,1",
    "guaranteedA2nBandwidth": 1,
    "hcosMgmtControl": false,
    "independentA2nShaperControl": false,
    "innerTagPrioControl": false,
    "loopAvoidanceInstance": "1",
    "maximumA2nBandwidth": 1,
    "multiCosControl": false,
    "outerTagPrioControl": false,
    "policingControl": false,
    "policingDirection": "1",
    "preserveStagProperties": false,
    "prioMapProfile": "1",
    "protectAccessLearningControl": "1",
    "protectNetworkLearningControl": "1",
    "sesFlrThreshold": 0,
    "stagControl": "1",
    "stagMatchRxPrio": false,
    "stagVlan": "1-*",
    "tableFullAction": "1",
    "untaggedMemberControl": false,
    "vlanMemberList": "1",
    "wfqGroupCos": 1,
    "wfqGroupEir": 1,
    "wfqSegmentationCos": 1,
    "adminState": "1",
    "dnm": "1"
}
EOF
)"
doCurl "${urlPrefix}${url}" "${data}"
echo $url
}
function createFpAcl()
{
echo >&2
echo "*       create fp acl $1 $2" >&2        
local fpUrl=$1
local aclId=$2
local url="${fpUrl}/fpacl/${aclId}"
local data="$(cat <<EOF
{
    "aclId": ${aclId},
    "action": "1",
    "active": false,
    "alias": "1",
    "cos": 0,
    "cosOverrideEnabled": false,
    "dstIpv4AddressEnabled": false,
    "dstIpv4AddressHighLimit": "1.1.1.1",
    "dstIpv4AddressLowLimit": "1.1.1.1",
    "dstIpv6Address": "::",
    "dstIpv6AddressEnabled": false,
    "dstIpv6AddressPrefixLen": 0,
    "dstMacAddress": "11:22:33:44:55:66",
    "dstMacAddressEnabled": false,
    "dstMacAddressMask": "ff:ff:ff:ff:ff:ff",
    "dstPortEnabled": false,
    "dstPortHighLimit": 1,
    "dstPortLowLimit": 1,
    "dynamicSrcIpv4AddressEnabled": false,
    "dynamicSrcMacAddressEnabled": false,
    "ethertype": 1,
    "ethertypeEnabled": false,
    "innerVlanPcpEnabled": false,
    "innerVlanPcpHighLimit": 1,
    "innerVlanPcpLowLimit": 1,
    "innerVlanVidEnabled": false,
    "innerVlanVidHighLimit": 1,
    "innerVlanVidLowLimit": 1,
    "ipv4PriEnabled": false,
    "ipv4PriHighLimit": 1,
    "ipv4PriLowLimit": 1,
    "outerVlanDei": 0,
    "outerVlanDeiEnabled": false,
    "outerVlanPcpEnabled": false,
    "outerVlanPcpHighLimit": 1,
    "outerVlanPcpLowLimit": 1,
    "outerVlanVidEnabled": false,
    "outerVlanVidHighLimit": 1,
    "outerVlanVidLowLimit": 1,
    "priority": 1,
    "protocolEnabled": false,
    "protocolNumber": 1,
    "srcIpv4AddressEnabled": false,
    "srcIpv4AddressHighLimit": "1.1.1.1",
    "srcIpv4AddressLowLimit": "1.1.1.1",
    "srcIpv6Address": "::",
    "srcIpv6AddressEnabled": false,
    "srcIpv6AddressPrefixLen": 0,
    "srcMacAddress": "11:22:33:44:55:66",
    "srcMacAddressEnabled": false,
    "srcMacAddressMask": "ff:ff:ff:ff:ff:ff",
    "srcPortEnabled": false,
    "srcPortHighLimit": 1,
    "srcPortLowLimit": 1,
    "tcpFlags": 1,
    "tcpFlagsEnabled": false,
    "adminState": "1",
    "dnm": "1"
}
EOF
)"
doCurl "${urlPrefix}${url}" "${data}"
echo $url
}
if [ $# -ne 1 ]; then
    echo "usage: $0 <login|token>" 
    exit;
fi
if [ "-$1" = "-login" ]; then
    login
    exit;
else
    token=$1
fi


createSubnetwork
createNetworkElement
url=$(createShelf)
url=$(createSlot "${url}" 1)

fpid=1
port=1
poid=1
shid=1
if [ -z "${PORTSTART}" ]; then
PORTSTART=1
PORTEND=60
fi

# 60 port, 12000 fp, 12000 police, 12000 shaper
for port in $(seq ${PORTSTART} ${PORTEND}); do
    time (
    porturl=$(createPort "${url}" ${port})
    for fpid in $(seq 1 200); do
        fpUrl=$(createFp "${porturl}" ${fpid})
        for poid in $(seq 1); do
            createFpPolicer ${fpUrl} ${poid}
        done
        for shid in $(seq 1); do
            createFpShaper ${fpUrl} ${shid}
        done
#        createFpAcl ${fpUrl} 1
    done
    )>&2
done

# 4 port, 4000 fp, 4000 shaper
for port in $(seq 61 64); do
    time (
    porturl=$(createPort "${url}" ${port})
    for fpid in $(seq 1 1000); do
        fpUrl=$(createFp "${porturl}" ${fpid})
        for shid in $(seq 1); do
            createFpShaper ${fpUrl} ${shid}
        done
#        createFpAcl ${fpUrl} 1

    done
    )>&2
done

# 8000 flow
for flow in $(seq 1 8000); do
    time (
    createFlow "${url}" ${flow}
    )>&2
done

#url="/f3sn/f3ne/1/f3shelf/1/f3slot/1/f3card/f3eoe/f3l2port/1,1"


