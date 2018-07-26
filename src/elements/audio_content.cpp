#include "adm/elements/audio_content.hpp"
#include <algorithm>
#include <functional>
#include "adm/document.hpp"
#include "adm/elements/loudness_metadata.hpp"
#include "adm/elements/private/parent_attorneys.hpp"
#include "adm/elements/private/auto_parent.hpp"
#include "adm/utilities/element_io.hpp"
#include "adm/utilities/id_assignment.hpp"

namespace adm {

  // ---- Getter ---- //

  ContentKind AudioContent::get(DispatchTag<ContentKind>) const {
    if (has<NonDialogueContentKind>()) {
      return ContentKind(storage_.get<NonDialogueContentKind>());
    } else if (has<DialogueContentKind>()) {
      return ContentKind(storage_.get<DialogueContentKind>());
    } else {
      return ContentKind(storage_.get<MixedContentKind>());
    }
  }

  bool AudioContent::has(DispatchTag<ContentKind>) const {
    return storage_.has<NonDialogueContentKind>() ||
           storage_.has<DialogueContentKind>() ||
           storage_.has<MixedContentKind>();
  }

  // ---- Setter ---- //
  void AudioContent::set(AudioContentId id) {
    if (isUndefined(id)) {
      storage_.set(id);
      return;
    }
    if (getParent().lock() != nullptr && getParent().lock()->lookup(id)) {
      throw std::runtime_error("id already in use");
    }
    storage_.set(id);
  }

  void AudioContent::set(DialogueId id) {
    if (storage_.has<DialogueId>() && storage_.get<DialogueId>() == id) {
      return;
    }
    unset<DialogueId>();
    storage_.set(id);
    if (id == Dialogue::NON_DIALOGUE) {
      storage_.set(NonDialogueContent::UNDEFINED);
    } else if (id == Dialogue::DIALOGUE) {
      storage_.set(DialogueContent::UNDEFINED);
    } else if (id == Dialogue::MIXED) {
      storage_.set(MixedContent::UNDEFINED);
    }
  }
  void AudioContent::set(ContentKind kind) {
    if (kind.which() == 0) {
      set(boost::get<NonDialogueContentKind>(kind));
    } else if (kind.which() == 1) {
      set(boost::get<DialogueContentKind>(kind));
    } else if (kind.which() == 2) {
      set(boost::get<MixedContentKind>(kind));
    }
  }
  void AudioContent::set(NonDialogueContentKind kind) {
    unset<DialogueId>();
    storage_.set(kind);
    storage_.set(Dialogue::NON_DIALOGUE);
  }
  void AudioContent::set(DialogueContentKind kind) {
    unset<DialogueId>();
    storage_.set(kind);
    storage_.set(Dialogue::DIALOGUE);
  }
  void AudioContent::set(MixedContentKind kind) {
    unset<DialogueId>();
    storage_.set(kind);
    storage_.set(Dialogue::MIXED);
  }

  // ---- Unsetter ---- //

  void AudioContent::unset(AudioContent::DispatchTag<DialogueId>) {
    storage_.unset<DialogueId>();
    storage_.unset<NonDialogueContentKind>();
    storage_.unset<DialogueContentKind>();
    storage_.unset<MixedContentKind>();
  }
  void AudioContent::unset(AudioContent::DispatchTag<NonDialogueContentKind>) {
    unset<DialogueId>();
  }
  void AudioContent::unset(AudioContent::DispatchTag<DialogueContentKind>) {
    unset<DialogueId>();
  }
  void AudioContent::unset(AudioContent::DispatchTag<MixedContentKind>) {
    unset<DialogueId>();
  }

  // ---- References ---- //
  bool AudioContent::addReference(std::shared_ptr<AudioObject> object) {
    autoParent(shared_from_this(), object);
    if (getParent().lock() != object->getParent().lock()) {
      throw std::runtime_error(
          "AudioContent cannot refer to an AudioObject in a different "
          "document");
    }
    auto it = std::find(audioObjects_.begin(), audioObjects_.end(), object);
    if (it == audioObjects_.end()) {
      audioObjects_.push_back(object);
      return true;
    } else {
      return false;
    }
  }

  void AudioContent::removeReference(std::shared_ptr<AudioObject> object) {
    auto it = std::find(audioObjects_.begin(), audioObjects_.end(), object);
    if (it != audioObjects_.end()) {
      audioObjects_.erase(it);
    }
  }

  void AudioContent::disconnectReferences() { clearReferences<AudioObject>(); }

  void AudioContent::clearReferences(
      detail::ParameterTraits<AudioObject>::tag) {
    return audioObjects_.clear();
  }

  // ---- Common ---- //
  void AudioContent::print(std::ostream& os) const {
    os << get<AudioContentId>();
    os << " (audioContentName=";
    os << get<AudioContentName>();
    if (has<AudioContentLanguage>()) {
      os << ", audioContentLanguage=" << get<AudioContentLanguage>();
    }
    if (has<LoudnessMetadata>()) {
      os << ", loudnessMetadata=" << get<LoudnessMetadata>();
    }
    if (has<DialogueId>()) {
      os << ", dialogueId=" << get<DialogueId>();
    }
    if (has<ContentKind>()) {
      os << ", contentKind=" << get<ContentKind>();
    }
    if (has<NonDialogueContentKind>()) {
      os << ", nonDialogueContentKind=" << get<NonDialogueContentKind>();
    }
    if (has<DialogueContentKind>()) {
      os << ", dialogueContentKind=" << get<DialogueContentKind>();
    }
    if (has<MixedContentKind>()) {
      os << ", mixedContentKind=" << get<MixedContentKind>();
    }
    os << ")";
  }

  void AudioContent::setParent(std::weak_ptr<Document> document) {
    parent_ = document;
  }
  std::weak_ptr<Document> AudioContent::getParent() const { return parent_; }

  std::shared_ptr<AudioContent> AudioContent::copy() const {
    auto audioContentCopy =
        std::shared_ptr<AudioContent>(new AudioContent(*this));
    audioContentCopy->setParent(std::weak_ptr<Document>());
    audioContentCopy->disconnectReferences();
    return audioContentCopy;
  }

  AudioContent::AudioContent(AudioContentName name) { storage_.set(name); }

}  // namespace adm
