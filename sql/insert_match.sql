INSERT INTO league_name.fixtures (fixture_date, home_id, away_id, home_score, away_score)
SELECT 
  CAST(f.fixture_date AS DATE) AS fixture_date,
  th.team_id,
  ta.team_id,
  f.home_score,
  f.away_score
FROM (
            SELECT 'YYYY-MM-DD' AS fixture_date, 'Team 1' AS home_team, 'Team 2' AS away_team, 1 AS home_score, 1 AS away_score
  UNION ALL SELECT 'YYYY-MM-DD' AS fixture_date, 'Team 3' AS home_team, 'Team 4' AS away_team, 3 AS home_score, 2 AS away_score
  UNION ALL SELECT 'YYYY-MM-DD' AS fixture_date, 'Team 5' AS home_team, 'Team 6' AS away_team, 2 AS home_score, 2 AS away_score;
) AS f
JOIN league_name.teams AS th ON th.name = f.home_team
JOIN league_name.teams AS ta ON ta.name = f.away_team;

