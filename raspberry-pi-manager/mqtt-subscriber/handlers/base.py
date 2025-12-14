from abc import ABC, abstractmethod
import logging
import json

class BaseHandler(ABC):
    def __init__(self):
        self.logger = logging.getLogger(self.__class__.__name__)

    @staticmethod
    def _match_pattern(pattern: str, topic: str) -> bool:
        """
        Check if a given topic matches the specified pattern.

        Args:
            pattern (str): The topic pattern, which may include wildcards '+' and '#'.
            topic (str): The actual topic to check against the pattern.

        Returns:
            bool: True if the topic matches the pattern, False otherwise.
        """
        topic_parts = topic.split('/')
        pattern_parts = pattern.split('/')

        for i, pattern_part in enumerate(pattern_parts):
            if pattern_part == '#':
                return True
            if pattern_part == '+':
                if i >= len(topic_parts):
                    return False
                continue
            if i >= len(topic_parts) or pattern_part != topic_parts[i]:
                return False
        
        return len(topic_parts) == len(pattern_parts)
    
    @abstractmethod
    def get_topic_patterns(self) -> list[str]:
        """
        Returns a list of topic patterns that this handler can process.

        Returns:
            list[str]: List of topic patterns.
        """
        pass

    @abstractmethod
    def can_handle(self, topic: str) -> bool:
        """
        Determines if this handler can process the given topic.

        Args:
            topic (str): The MQTT topic to check.
        
        Returns:
            bool: True if the handler can process the topic, False otherwise.
        """
        return any(self._match_pattern(pattern, topic) for pattern in self.get_topic_patterns())
    
    @abstractmethod
    def process(self, topic: str, payload: str) -> None:
        """
        Process the incoming MQTT message.

        Args:
            topic (str): The MQTT topic of the message.
            payload (str): The payload of the message.
        """
        pass

    def handle(self, topic: str, raw_payload: str) -> bool:
        """
        Handle the incoming MQTT message if this handler can process it.

        Args:
            topic (str): The MQTT topic of the message.
            raw_payload (str): The raw payload of the message (JSON string).
        
        Returns:
            bool: True if the handler can process the topic, False otherwise.
        """
        try:
            if not self.can_handle(topic):
                return False
            
            # Parse payload (assuming JSON format)
            payload = json.loads(raw_payload)

            self.logger.info(f"Processing message on topic: {topic}")
            self.logger.debug(f"Payload: {payload}")

            # Process the message
            self.process(topic, payload)

            return True
        except json.JSONDecodeError as e:
            self.logger.error(f"Failed to decode JSON payload: {e} on topic: {topic}")
            return False
        except Exception as e:
            self.logger.error(f"Error processing message on topic: {topic}: {e}")
            return False