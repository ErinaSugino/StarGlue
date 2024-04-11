#ifndef STAR_UNIVERSE_CLIENT_HPP
#define STAR_UNIVERSE_CLIENT_HPP

#include "StarMaybe.hpp"
#include "StarHostAddress.hpp"
#include "StarGameTimers.hpp"
#include "StarCelestialParameters.hpp"
#include "StarChatTypes.hpp"
#include "StarWarping.hpp"
#include "StarAiTypes.hpp"
#include "StarSky.hpp"
#include "StarUniverseConnection.hpp"

namespace Star {

STAR_CLASS(WorldTemplate);
STAR_CLASS(ClientContext);
STAR_CLASS(Sky);
STAR_STRUCT(Packet);
STAR_CLASS(WorldClient);
STAR_CLASS(SystemWorldClient);
STAR_CLASS(Player);
STAR_CLASS(PlayerStorage);
STAR_CLASS(Statistics);
STAR_CLASS(Clock);
STAR_CLASS(CelestialLog);
STAR_CLASS(CelestialSlaveDatabase);
STAR_CLASS(CelestialDatabase);
STAR_CLASS(JsonRpcInterface);
STAR_CLASS(TeamClient);
STAR_CLASS(QuestManager);

STAR_CLASS(UniverseClient);

class UniverseClient {
public:
  UniverseClient(PlayerStoragePtr playerStorage, StatisticsPtr statistics);
  ~UniverseClient();

  void setMainPlayer(PlayerPtr player);
  PlayerPtr mainPlayer() const;

  // Returns error if connection failed
  Maybe<String> connect(UniverseConnection connection, bool allowAssetsMismatch, String const& account = "", String const& password = "");
  bool isConnected() const;
  void disconnect();
  Maybe<String> disconnectReason() const;

  // WorldClient may be null if the UniverseClient is not connected.
  WorldClientPtr worldClient() const;
  SystemWorldClientPtr systemWorldClient() const;

  // Updates internal world client in addition to handling universe level
  // commands.
  void update();

  Maybe<BeamUpRule> beamUpRule() const;
  bool canBeamUp() const;
  bool canBeamDown(bool deploy = false) const;
  bool canBeamToTeamShip() const;
  bool canTeleport() const;

  void warpPlayer(WarpAction const& warpAction, bool animate = true, String const& animationType = "default", bool deploy = false);
  void flyShip(Vec3I const& system, SystemLocation const& destination);

  CelestialDatabasePtr celestialDatabase() const;

  CelestialCoordinate shipCoordinate() const;

  bool playerOnOwnShip() const;

  WorldId playerWorld() const;
  bool isAdmin() const;
  // If the player is in a multi person team returns the team uuid, or if the
  // player is by themselves returns the player uuid.
  Uuid teamUuid() const;

  WorldTemplateConstPtr currentTemplate() const;
  SkyConstPtr currentSky() const;
  bool flying() const;

  void sendChat(String const& text, ChatSendMode sendMode);
  List<ChatReceivedMessage> pullChatMessages();

  uint16_t players();
  uint16_t maxPlayers();

  ClockConstPtr universeClock() const;
  CelestialLogConstPtr celestialLog() const;
  JsonRpcInterfacePtr rpcInterface() const;
  ClientContextPtr clientContext() const;
  TeamClientPtr teamClient() const;
  QuestManagerPtr questManager() const;
  PlayerStoragePtr playerStorage() const;
  StatisticsPtr statistics() const;

  bool paused() const;

private:
  struct ServerInfo {
    uint16_t players;
    uint16_t maxPlayers;
  };

  void setPause(bool pause);

  void handlePackets(List<PacketPtr> const& packets);
  void reset();

  PlayerStoragePtr m_playerStorage;
  StatisticsPtr m_statistics;
  PlayerPtr m_mainPlayer;

  bool m_pause;
  ClockPtr m_universeClock;
  WorldClientPtr m_worldClient;
  SystemWorldClientPtr m_systemWorldClient;
  Maybe<UniverseConnection> m_connection;
  Maybe<ServerInfo> m_serverInfo;

  CelestialSlaveDatabasePtr m_celestialDatabase;
  ClientContextPtr m_clientContext;
  TeamClientPtr m_teamClient;

  QuestManagerPtr m_questManager;

  WarpAction m_pendingWarp;
  GameTimer m_warpDelay;
  Maybe<GameTimer> m_warpCinemaCancelTimer;

  Maybe<WarpAction> m_warping;
  bool m_respawning;
  GameTimer m_respawnTimer;

  int64_t m_storageTriggerDeadline;

  List<ChatReceivedMessage> m_pendingMessages;

  Maybe<String> m_disconnectReason;
};

}

#endif
