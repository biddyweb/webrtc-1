﻿// Class1.cpp
#include "GlobalObserver.h"
#include "peerconnectioninterface.h"
#include "Marshalling.h"
#include <ppltasks.h>

using namespace webrtc_winrt_api_internal;
using namespace Platform;

void GlobalObserver::SetPeerConnection(webrtc_winrt_api::RTCPeerConnection^ pc)
{
  _pc = pc;
}

// Triggered when the SignalingState changed.
void GlobalObserver::OnSignalingChange(
  webrtc::PeerConnectionInterface::SignalingState new_state)
{
}

// Triggered when SignalingState or IceState have changed.
// TODO(bemasc): Remove once callers transition to OnSignalingChange.
void GlobalObserver::OnStateChange(StateType state_changed)
{
}

// Triggered when media is received on a new stream from remote peer.
void GlobalObserver::OnAddStream(webrtc::MediaStreamInterface* stream)
{
  if (_pc != nullptr)
  {
    // TODO: Register for changes and trigger OnTrack then.
    auto evt = ref new webrtc_winrt_api::RTCTrackEvent();
    _pc->OnTrack(evt);
  }
}

// Triggered when a remote peer close a stream.
void GlobalObserver::OnRemoveStream(webrtc::MediaStreamInterface* stream)
{

}

// Triggered when a remote peer open a data channel.
void GlobalObserver::OnDataChannel(webrtc::DataChannelInterface* data_channel)
{

}

// Triggered when renegotiation is needed, for example the ICE has restarted.
void GlobalObserver::OnRenegotiationNeeded()
{
  if (_pc != nullptr)
  {
    _pc->OnNegotiationNeeded();
  }
}

// Called any time the IceConnectionState changes
void GlobalObserver::OnIceConnectionChange(
  webrtc::PeerConnectionInterface::IceConnectionState new_state)
{

}

// Called any time the IceGatheringState changes
void GlobalObserver::OnIceGatheringChange(
  webrtc::PeerConnectionInterface::IceGatheringState new_state)
{

}

// New Ice candidate have been found.
void GlobalObserver::OnIceCandidate(const webrtc::IceCandidateInterface* candidate)
{
  if (_pc != nullptr && candidate != nullptr)
  {
    auto evt = ref new webrtc_winrt_api::RTCPeerConnectionIceEvent();
    webrtc_winrt_api::RTCIceCandidate^ cxCandidate;
    ToCx(*candidate, &cxCandidate);
    evt->Candidate = cxCandidate;
    _pc->OnIceCandidate(evt);
  }
}

// TODO(bemasc): Remove this once callers transition to OnIceGatheringChange.
// All Ice candidates have been found.
void GlobalObserver::OnIceComplete()
{
}

//============================================================================

CreateSdpObserver::CreateSdpObserver()
  : _sdi(nullptr)
  , _callbackHappened(true, false)
{

}

void CreateSdpObserver::OnSuccess(webrtc::SessionDescriptionInterface* desc)
{
  _sdi = desc;
  _callbackHappened.Set();
}

void CreateSdpObserver::OnFailure(const std::string& error)
{
  _error = error;
  _callbackHappened.Set();
}

void CreateSdpObserver::Wait(
  std::function<void(webrtc::SessionDescriptionInterface*)> onSuccess,
  std::function<void(const std::string&)> onFailure)
{
  _callbackHappened.Wait(rtc::Event::kForever);
  if (_sdi != nullptr)
  {
      onSuccess(_sdi);
  }
  else
  {
      onFailure(_error);
  }
}

//============================================================================

SetSdpObserver::SetSdpObserver()
  : _callbackHappened(true, false)
{

}

void SetSdpObserver::OnSuccess()
{
  _callbackHappened.Set();
}

void SetSdpObserver::OnFailure(const std::string& error)
{
  _error = error;
  _callbackHappened.Set();
}

void SetSdpObserver::Wait(
  std::function<void()> onSuccess,
  std::function<void(const std::string&)> onFailure)
{
  _callbackHappened.Wait(rtc::Event::kForever);
  if (_error.empty())
  {
    onSuccess();
  }
  else
  {
    onFailure(_error);
  }
}
