#include "adm/elements/audio_programme.hpp"
#include "adm/document.hpp"
#include "adm/elements/time.hpp"
#include "adm/elements/audio_content.hpp"
#include "adm/elements/loudness_metadata.hpp"
#include "adm/elements/private/parent_attorneys.hpp"
#include "adm/elements/private/auto_parent.hpp"
#include "adm/elements/private/auto_parent.hpp"
#include "adm/utilities/element_io.hpp"
#include "adm/utilities/id_assignment.hpp"

namespace adm {

  // ---- Setter ---- //
  void AudioProgramme::set(AudioProgrammeId id) {
    if (isUndefined(id)) {
      storage_.set(id);
      return;
    }
    if (getParent().lock() != nullptr && getParent().lock()->lookup(id)) {
      throw std::runtime_error("id already in use");
    }
    storage_.set(id);
  }

  // ---- References ---- //
  bool AudioProgramme::addReference(std::shared_ptr<AudioContent> content) {
    autoParent(shared_from_this(), content);
    if (getParent().lock() != content->getParent().lock()) {
      throw std::runtime_error(
          "AudioProgramme cannot refer to an AudioContent in a different "
          "document");
    }
    auto it = std::find(audioContents_.begin(), audioContents_.end(), content);
    if (it == audioContents_.end()) {
      audioContents_.push_back(content);
      return true;
    } else {
      return false;
    }
  }

  void AudioProgramme::removeReference(std::shared_ptr<AudioContent> content) {
    auto it = std::find(audioContents_.begin(), audioContents_.end(), content);
    if (it != audioContents_.end()) {
      audioContents_.erase(it);
    }
  }

  void AudioProgramme::disconnectReferences() {
    clearReferences<AudioContent>();
  }

  void AudioProgramme::clearReferences(
      detail::ParameterTraits<AudioContent>::tag) {
    audioContents_.clear();
  }

  // ---- Common ---- //
  void AudioProgramme::print(std::ostream& os) const {
    os << get<AudioProgrammeId>();
    os << " (audioProgrammeName=";
    os << get<AudioProgrammeName>();
    if (has<AudioProgrammeLanguage>()) {
      os << ", audioProgrammeLanguage=" << get<AudioProgrammeLanguage>();
    }
    if (has<Start>()) {
      os << ", start=" << formatTimecode(get<Start>().get());
    }
    if (has<End>()) {
      os << ", end=" << formatTimecode(get<End>().get());
    }
    if (has<LoudnessMetadata>()) {
      os << ", loudnessMetadata=" << get<LoudnessMetadata>();
    }
    if (has<MaxDuckingDepth>()) {
      os << ", maxDuckingDepth=" << get<MaxDuckingDepth>();
    }
    if (has<AudioProgrammeReferenceScreen>()) {
      os << ", audioProgrammeReferenceScreen="
         << get<AudioProgrammeReferenceScreen>();
    }
    os << ")";
  }

  void AudioProgramme::setParent(std::weak_ptr<Document> document) {
    parent_ = document;
  }
  std::weak_ptr<Document> AudioProgramme::getParent() const { return parent_; };

  std::shared_ptr<AudioProgramme> AudioProgramme::copy() const {
    auto audioProgrammeCopy =
        std::shared_ptr<AudioProgramme>(new AudioProgramme(*this));
    audioProgrammeCopy->setParent(std::weak_ptr<Document>());
    audioProgrammeCopy->disconnectReferences();
    return audioProgrammeCopy;
  }

  AudioProgramme::AudioProgramme(AudioProgrammeName name) {
    storage_.set(name);
  };
}  // namespace adm
