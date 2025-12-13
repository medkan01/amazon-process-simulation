-- Initialize TimescaleDB Extension
create extension if not exists timescaledb;

-- Set timescaledb time zone to UTC
set timezone = 'UTC';
alter database amazon_process_simulation_db set timezone = 'UTC';

do $$
begin
    raise notice 'TimescaleDB initialized successfully. Timezone configuration : %', current_setting('timezone');
end
$$;