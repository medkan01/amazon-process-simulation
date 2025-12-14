FROM python:3.12-slim
WORKDIR /app

# Update pip
RUN pip install --upgrade pip

# Install required Python packages
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Copy the MQTT subscriber script into the container
COPY mqtt_subscriber.py .

# Set the entry point to run the MQTT subscriber script
CMD ["python", "mqtt_subscriber.py"]