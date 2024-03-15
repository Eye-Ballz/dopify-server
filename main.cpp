#include "crow/middlewares/cors.h"
#include <crow.h>
#include <iostream>
#include <string>
#include <vector>

struct song {
  int pointer;
  std::string name;
  std::string artist;
  std::string cover_url;
  int year;
};
song parse_song_json_data(std::string json) {
  crow::json::rvalue s1_json_rvalue = crow::json::load(json);
  song Song;
  Song.pointer = s1_json_rvalue["pointer"].i();
  Song.name = s1_json_rvalue["name"].s();
  Song.artist = s1_json_rvalue["artist"].s();
  Song.cover_url = s1_json_rvalue["cover_url"].s();
  Song.year = s1_json_rvalue["year"].i();
  return Song;
}
std::string song_struct_to_json(song Song) {
  crow::json::wvalue Song_json;
  Song_json["pointer"] = Song.pointer;
  Song_json["name"] = Song.name;
  Song_json["artist"] = Song.artist;
  Song_json["cover_url"] = Song.cover_url;
  Song_json["year"] = Song.year;
  return Song_json.dump();
}
void writeStringToFile(const std::string &str, const std::string &filename) {
  std::ofstream file(filename);
  if (file.is_open()) {
    file << str;
    file.close();
  } else {
    std::cerr << "Unable to open file: " << filename << std::endl;
  }
}

std::vector<song> parseSongs(const std::string &filename) {
  std::vector<song> songs;

  std::ifstream file(filename);
  std::string str((std::istreambuf_iterator<char>(file)),
                  std::istreambuf_iterator<char>());

  auto x = crow::json::load(str);

  if (!x) {
    std::cerr << "Error parsing JSON file." << std::endl;
    return songs;
  }

  for (const auto &element : x) {
    song Song;
    Song.pointer = element["pointer"].i();
    Song.name = element["name"].s();
    Song.artist = element["artist"].s();
    Song.cover_url = element["cover_url"].s();
    Song.year = element["year"].i();
    songs.push_back(Song);
  }

  return songs;
}
std::vector<song> songs;
int main() {

  crow::App<crow::CORSHandler> app;

  // Customize CORS
  auto &cors = app.get_middleware<crow::CORSHandler>();

  // clang-format off
    cors
      .global()
        .headers("X-Custom-Header", "Upgrade-Insecure-Requests")
        .methods("POST"_method, "GET"_method)
      .prefix("/cors")
        .origin("example.com")
      .prefix("/nocors")
        .ignore();

    songs = parseSongs("/home/aniket/code/dopify-server/songs.json");


    for(const auto& s : songs) {
        std::cout << "Pointer: " << s.pointer << "\n"
                  << "Name: " << s.name << "\n"
                  << "Artist: " << s.artist << "\n"
                  << "Cover URL: " << s.cover_url << "\n"
                  << "Year: " << s.year << "\n\n";
    }

    
    // crow::SimpleApp app;
    CROW_ROUTE(app, "/songs/<int>")([](int pointer){
        if (pointer > songs.size()){
            return std::string("END OF DATA");
        }
        else{
            std::cout<< song_struct_to_json(songs[pointer]);
            return song_struct_to_json(songs[pointer]);
        }
    });

    app.port(18080).multithreaded().run();
    return 0;
}