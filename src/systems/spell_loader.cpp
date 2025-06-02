#include "spell_loader.hpp"

// Function to load spells from a directory and parse them into the registry
void loadSpells(entt::registry& registry, const std::string& path) {
	try {
		for (const auto& entry : fs::recursive_directory_iterator(path)) {
			if (entry.path().extension() != ".json") continue;

			try {
				std::ifstream file(entry.path());
				json j;
				file >> j;

				auto spell = parseSpell(j);
				auto id = parseIdentifier(j);
				auto stats = parseStats(j);
				auto shape = parseShape(j);

				if (printSpellParsingErrors(entry.path().string(), spell, id, stats, shape)) continue;

				auto entity = registry.create();
				registry.emplace<Spell>(entity, *spell);
				registry.emplace<SpellIdentifier>(entity, *id);
				registry.emplace<SpellStats>(entity, *stats);
				registry.emplace<SpellShape>(entity, *shape);
			}
			catch (const json::exception& e) {
				std::cerr << "JSON error in " << entry.path() << ": " << e.what() << "\n";
			}
		}
	}	catch (const std::exception& e) {
		std::cerr << "Spell directory path error: " << e.what() << "\n";
	}
}