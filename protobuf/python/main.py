import os
from proto.common.common_pb2 import Payload
def main():
    payload = Payload()
    payload.val_int = 2
    payload.val_str = "test"
    payload.val_msg.mock_int = 3
    payload.val_arr.add().mock_int = 4
    payload.val_arr.add().mock_int = 5
    print(payload)

if __name__ == "__main__":
    main()