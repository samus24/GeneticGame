#ifndef BOOK_RESOURCEHOLDER_HPP
#define BOOK_RESOURCEHOLDER_HPP

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>


template <typename Resource, typename Identifier>
class ResourceHolder
{
	public:
		void						load(Identifier id, const std::string& filename);

		template <typename Parameter>
		void						load(Identifier id, const std::string& filename, const Parameter& secondParam);

		Resource&					get(Identifier id);
		const Resource&				get(Identifier id) const;


	protected:
		void						insertResource(Identifier id, std::unique_ptr<Resource> resource);


	protected:
		std::map<Identifier, std::unique_ptr<Resource>>	mResourceMap;
};

template <typename Resource, typename Identifier>
class RealMusicHolder : public ResourceHolder<Resource, Identifier>
{
public:
	void						load(Identifier id, const std::string& filename);

	template <typename Parameter>
	void						load(Identifier id, const std::string& filename, const Parameter& secondParam);
};

#include "ResourceHolder.inl"
#endif // BOOK_RESOURCEHOLDER_HPP
