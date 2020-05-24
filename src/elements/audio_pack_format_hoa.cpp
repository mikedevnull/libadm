#include "adm/elements/audio_pack_format_hoa.hpp"
#include "adm/utilities/element_io.hpp"
#include "adm/utilities/id_assignment.hpp"

#include <iostream>

namespace adm {
  // audio_pack_format_hoa::audio_pack_format_hoa(){
  //}

  // ---- Defaults ---- //
  namespace {
    const Normalization normalizationDefault{"SN3D"};
    const NfcRefDist nfcRefDistDefault{0};
    const ScreenRef screenRefDefault{false};
  }  // namespace

  Normalization AudioPackFormatHoaDefaults::create(Normalization::tag) {
    return normalizationDefault;
  }
  NfcRefDist AudioPackFormatHoaDefaults::create(NfcRefDist::tag) {
    return nfcRefDistDefault;
  }
  ScreenRef AudioPackFormatHoaDefaults::create(ScreenRef::tag) {
    return screenRefDefault;
  }

  // ---- Common ---- //
  void AudioPackFormatHoa::print(std::ostream& os) const {
    os << get<AudioPackFormatId>();
    os << " (";
    os << "audioPackFormatName=" << get<AudioPackFormatName>();
    os << ", typeLabel=" << formatTypeLabel(get<TypeDescriptor>());
    os << ", typeDefinition=" << formatTypeDefinition(get<TypeDescriptor>());
    if (has<Importance>()) {
      os << ", importance=" << get<Importance>();
    }
    if (has<AbsoluteDistance>()) {
      os << ", absoluteDistance=" << get<AbsoluteDistance>();
    }
    if (has<ScreenRef>()) {
      os << ", screenRef=" << get<ScreenRef>();
    }
    if (has<Normalization>()) {
      os << ", normalization=" << get<Normalization>();
    }
    if (has<NfcRefDist>()) {
      os << ", nfcRefDist=" << get<NfcRefDist>();
    }
    os << ")";
  }

  AudioPackFormatHoa::AudioPackFormatHoa(AudioPackFormatName name)
      : AudioPackFormat(name, adm::TypeDefinition::HOA) {}
}  // namespace adm
