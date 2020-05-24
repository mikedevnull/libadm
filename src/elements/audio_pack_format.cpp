#include "adm/elements/audio_pack_format.hpp"
#include "adm/document.hpp"
#include "adm/elements/private/parent_attorneys.hpp"
#include "adm/elements/private/auto_parent.hpp"
#include "adm/utilities/element_io.hpp"
#include "adm/utilities/id_assignment.hpp"

#include <iostream>

namespace adm {

  // ---- Setter ---- //
  void AudioPackFormat::set(AudioPackFormatId id) {
    if (isUndefined(id)) {
      storage_.set(id);
      return;
    }
    if (getParent().lock() != nullptr && getParent().lock()->lookup(id)) {
      throw std::runtime_error("id already in use");
    }
    if (id.get<TypeDescriptor>() == get<TypeDescriptor>()) {
      storage_.set(id);
    } else {
      std::stringstream errorString;
      errorString << "missmatch between TypeDefinition of AudioPackFormat ("
                  << formatTypeDefinition(get<TypeDescriptor>())
                  << ") and AudioPackFormatId ("
                  << formatTypeDefinition(id.get<TypeDescriptor>()) << ")";
      throw std::runtime_error(errorString.str());
    }
  }

  // ---- References ---- //

  bool AudioPackFormat::isAudioPackFormatReferenceCycle(
      std::shared_ptr<AudioPackFormat> destinationObject) {
    if (shared_from_this() == destinationObject) {
      return true;
    }
    for (auto packFormat : getReferences<AudioPackFormat>()) {
      if (packFormat->isAudioPackFormatReferenceCycle(destinationObject)) {
        return true;
      }
    }
    return false;
  }

  bool AudioPackFormat::addReference(
      std::shared_ptr<AudioChannelFormat> channelFormat) {
    autoParent(shared_from_this(), channelFormat);
    if (getParent().lock() != channelFormat->getParent().lock()) {
      throw std::runtime_error(
          "AudioPackFormat cannot refer to an AudioChannelFormat in a "
          "different document");
    }
    auto it = std::find(audioChannelFormats_.begin(),
                        audioChannelFormats_.end(), channelFormat);
    if (it == audioChannelFormats_.end()) {
      audioChannelFormats_.push_back(channelFormat);
      return true;
    } else {
      return false;
    }
  }

  bool AudioPackFormat::addReference(
      std::shared_ptr<AudioPackFormat> packFormat) {
    if (packFormat->isAudioPackFormatReferenceCycle(shared_from_this())) {
      throw std::runtime_error(
          "adding AudioPackFormat reference would create a reference cycle");
    }
    autoParent(shared_from_this(), packFormat);
    if (getParent().lock() != packFormat->getParent().lock()) {
      throw std::runtime_error(
          "AudioPackFormat cannot refer to an AudioPackFormat in a different "
          "document");
    }
    auto it = std::find(audioPackFormats_.begin(), audioPackFormats_.end(),
                        packFormat);
    if (it == audioPackFormats_.end()) {
      audioPackFormats_.push_back(packFormat);
      return true;
    } else {
      return false;
    }
  }

  void AudioPackFormat::removeReference(
      std::shared_ptr<AudioChannelFormat> object) {
    auto it = std::find(audioChannelFormats_.begin(),
                        audioChannelFormats_.end(), object);
    if (it != audioChannelFormats_.end()) {
      audioChannelFormats_.erase(it);
    }
  }

  void AudioPackFormat::removeReference(
      std::shared_ptr<AudioPackFormat> packFormat) {
    auto it = std::find(audioPackFormats_.begin(), audioPackFormats_.end(),
                        packFormat);
    if (it != audioPackFormats_.end()) {
      audioPackFormats_.erase(it);
    }
  }

  void AudioPackFormat::disconnectReferences() {
    clearReferences<AudioChannelFormat>();
    clearReferences<AudioPackFormat>();
  }

  void AudioPackFormat::clearReferences(
      detail::ParameterTraits<AudioChannelFormat>::tag) {
    audioChannelFormats_.clear();
  }

  void AudioPackFormat::clearReferences(
      detail::ParameterTraits<AudioPackFormat>::tag) {
    audioPackFormats_.clear();
  }

  // ---- Common ---- //
  void AudioPackFormat::print(std::ostream& os) const {
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
    os << ")";
  }

  void AudioPackFormat::setParent(std::weak_ptr<Document> document) {
    parent_ = document;
  }

  std::weak_ptr<Document> AudioPackFormat::getParent() const { return parent_; }

  std::shared_ptr<AudioPackFormat> AudioPackFormat::copy() const {
    auto audioPackFormatCopy =
        std::shared_ptr<AudioPackFormat>(new AudioPackFormat(*this));
    audioPackFormatCopy->setParent(std::weak_ptr<Document>());
    audioPackFormatCopy->disconnectReferences();
    return audioPackFormatCopy;
  }

  AudioPackFormat::AudioPackFormat(AudioPackFormatName name,
                                   TypeDescriptor channelType) {
    storage_.set(name);
    storage_.set(channelType);
  }

}  // namespace adm
